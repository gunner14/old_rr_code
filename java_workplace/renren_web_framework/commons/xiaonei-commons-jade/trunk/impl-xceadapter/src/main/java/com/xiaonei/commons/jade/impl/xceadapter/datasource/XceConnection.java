package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.SQLNonTransientConnectionException;
import java.sql.SQLSyntaxErrorException;

import net.paoding.rose.jade.jadeinterface.annotation.ShardParam;

import com.meidusa.amoeba.parser.dbobject.Column;
import com.meidusa.amoeba.parser.dbobject.Table;
import com.xiaonei.commons.jade.impl.jdbcwrapper.HAConnection;
import com.xiaonei.commons.jade.impl.util.JdbcUtils;
import com.xiaonei.commons.jade.impl.util.ThreadUtils;

/**
 * 实现依赖 : XceAdapter 的数据库连接。
 * 
 * @author han.liao
 */
public class XceConnection extends HAConnection {

    // 使用的数据源 
    protected XceDataSource ds;

    // 当前语句的分析信息
    protected XceParseInfo parseInfo;

    // 读写数据库状态
    protected boolean desireRead = true;

    // 强制访问写数据库
    protected boolean forceWrite;

    // 当前的配置信息
    protected XceRouter dbRouter, partitionRouter;

    // 关注的数据列
    protected Column dbColumn, partitionColumn;

    // 数据源, 散库名称
    protected String dbName, dbPattern;

    // 数据表, 散表名称
    protected String name, partition;

    /**
     * 创建: XceConnection 实例。
     * 
     * @param ds - 指定的数据源
     */
    public XceConnection(XceDataSource ds) {

        // BUGFIX: 检查  Catalog 有无全局配置, 如果有则设置为默认的  dbName. 
        XceDescriptor descriptor = ds.getCatalogDescriptor();

        if (descriptor != null) {

            // 采用  Catalog 的全局配置 
            dbName = descriptor.getDbName();

        } else {

            // 采用  Catalog 作为默认的  dbName. 
            dbName = ds.getCatalog();
        }

        // 采用数据源的强写模式
        forceWrite = ds.isForceWrite() || ThreadUtils.isForceWrite();

        // 采用数据源的只读模式
        readOnly = ds.isReadOnly();

        this.ds = ds;
    }

    @Override
    public String getCatalog() throws SQLException {

        return ds.getCatalog();
    }

    @Override
    public void setCatalog(String catalog) throws SQLException {

        // TODO: 覆盖底层的  Catalog 行为, 底层的行为是切换数据库, 这里我们需要切换  Catalog
        throw new UnsupportedOperationException("Can't switch catalog within connection.");
    }

    @Override
    public void setAutoCommit(boolean autoCommit) throws SQLException {

        if (autoCommit != this.autoCommit) {

            freeConnection(); // 切换使用的连接

            this.autoCommit = autoCommit;
        }
    }

    @Override
    public void setReadOnly(boolean readOnly) throws SQLException {

        if (readOnly != this.readOnly) {

            freeConnection(); // 切换使用的连接

            this.readOnly = readOnly;
        }
    }

    /**
     * 检查是否需要改变当前使用的数据库连接。
     * 
     * @param desireRead - 数据访问类型
     * 
     * @param dbName - 数据源的名称
     * @param dbPattern - 数据源的散库名称
     * 
     * @return 是否需要改变当前使用的数据库连接
     */
    private boolean changeConnection(boolean desireRead, String dbName, String dbPattern) {

        if (desireRead != this.desireRead) {
            return true;
        }

        if ((dbName != null) && !dbName.equals(this.dbName)) {
            return true;
        }

        if (dbPattern == null) {
            return (this.dbPattern != null);
        }

        return !dbPattern.equals(this.dbPattern);
    }

    /**
     * 检查是否需要改变当前使用的数据库连接。
     * 
     * @param dbPattern - 数据源的散库名称
     * 
     * @return 是否需要改变当前使用的数据库连接
     */
    private boolean changeConnection(String dbPattern) {

        if (dbPattern == null) {
            return (this.dbPattern != null);
        }

        return !dbPattern.equals(this.dbPattern);
    }

    /**
     * 使用所给的表名查找匹配的路由配置。
     * 
     * @param table - 查询的表名
     * 
     * @return 数据源的名称
     */
    private String findConfiguration(Table table) {

        XceDescriptor descriptor = ds.getDescriptor(table.getName());

        if (descriptor != null) {

            // 保存数据表
            this.name = table.getName();

            // 查找散表的路由
            XceRouter partitionRouter = descriptor.getPartitionRouter();

            if (partitionRouter != null) {

                String columnName = partitionRouter.getColumn();

                if (columnName != null) {

                    // 保存匹配的数据列
                    partitionColumn = new Column();
                    partitionColumn.setName(columnName.toUpperCase());
                    partitionColumn.setTable(table);
                }

                // 设置当前的路由信息
                this.partitionRouter = partitionRouter;

                // 输出调试信息
                if (logger.isDebugEnabled()) {
                    if (partitionColumn != null) {
                        logger.debug("Found partition router: ["
                                + partitionRouter.getClass().getName() // NL
                                + "] on " + partitionColumn);
                    } else {
                        logger.debug("Found partition router: ["
                                + partitionRouter.getClass().getName() // NL
                                + "] on " + name);
                    }
                }
            }

            // 查找散库的路由
            XceRouter dbRouter = descriptor.getDbRouter();

            if (dbRouter != null) {

                String columnName = dbRouter.getColumn();

                if (columnName != null) {

                    // 保存匹配的数据列
                    dbColumn = new Column();
                    dbColumn.setName(columnName.toUpperCase());
                    dbColumn.setTable(table);
                }

                // 设置当前的路由信息
                this.dbRouter = dbRouter;

                // 输出调试信息
                if (logger.isDebugEnabled()) {
                    if (dbColumn != null) {
                        logger.debug("Found DB router: [" // NL
                                + dbRouter.getClass().getName() + "] on " + dbColumn);
                    } else {
                        logger.debug("Found DB router: [" // NL
                                + dbRouter.getClass().getName() + "] on " + name);
                    }
                }
            }

            // 返回数据源名称
            return descriptor.getDbName();
        }

        return null;
    }

    @Override
    protected boolean analyzeSql(String sql) throws SQLException {

        if (logger.isDebugEnabled()) {
            logger.debug("Invoking analyzing: " + sql);
        }

        // 获取查询语句信息。
        parseInfo = ds.getParseInfo(sql);

        // 只有非事务的  SELECT 查询采用只读数据库连接。
        boolean desireRead = (!forceWrite) && // NL
                (readOnly || (autoCommit && parseInfo.isSelect()));

        String dbName = null, dbPattern = null;

        // 从查询的数据表获取路由配置。
        Table[] tables = parseInfo.getTables();

        if (tables != null) {

            // 查找散表配置
            if (parseInfo.isInsert()) {

                // 兼容 INSERT ... SELECT 查询, 后者查询条件来自 SELECT ... WHERE 数据表
                for (int i = tables.length - 1; i >= 0; i--) {
                    dbName = findConfiguration(tables[i]);
                    if (dbName != null) {
                        break;
                    }
                }
            } else {

                // 其他查询方式, 用第一个表的散表规则
                for (int i = 0; i < tables.length; i++) {
                    dbName = findConfiguration(tables[i]);
                    if (dbName != null) {
                        break;
                    }
                }
            }
        }

        if (partitionRouter != null) {

            // 用语句信息的常量进行散表。
            Object columnValue = null;

            if (partitionColumn != null) {
                columnValue = findShardParam(parseInfo, partitionColumn);
            }

            // 获得散表的名称
            partition = partitionRouter.doRoute(columnValue);
        }

        if (dbRouter != null) {

            // 用语句信息的常量进行散库。
            Object columnValue = null;

            if (dbColumn != null) {
                columnValue = findShardParam(parseInfo, dbColumn);
            }

            // 获得散库的名称
            dbPattern = dbRouter.doRoute(columnValue);
        }

        // 检查重定向参数是否变更 
        if (changeConnection(desireRead, dbName, dbPattern)) {

            // 如果是特殊查询，防止切换连接。
            if (parseInfo.isUnswitch()) {

                // 查询上一次插入的自动生成  ID 或者返回的记录行数， 不能切换连接
                // SELECT @@LAST_INSERT_ID
                // SELECT LAST_INSERT_ID()
                // SELECT FOUND_ROWS()
                return false;
            }

            // 切换使用的连接。
            freeConnection();

            // 保存重定向的参数
            this.desireRead = desireRead;

            // <code>null</code> 表示不需要重定向
            if (dbName != null) {
                this.dbName = dbName;
            }

            // 散库的名称, <code>null</code> 表示不需要散库
            this.dbPattern = dbPattern;

            return true;
        }

        return false;
    }

    // 查找散表参数
    protected static Object findShardParam(XceParseInfo parseInfo, Column column)
            throws SQLException {

        // 获取语句中的散表参数
        if (parseInfo.containsParam(column)) {
            return parseInfo.getParam(column);
        }

        // 检查外部提供的散表参数
        Column shardColumn = ThreadUtils.getShardColumn();
        if (shardColumn != null) {

            if (ShardParam.WIDECARD.equals(shardColumn.getName())) {
                // 匹配所有散表列
                return ThreadUtils.getShardParam();

            } else if (shardColumn.getTable() == null) {

                // 模糊的匹配散表列
                if (shardColumn.getName().equals(column.getName())) {
                    return ThreadUtils.getShardParam();
                }

            } else if (shardColumn.equals(column)) {
                // 精确的匹配散表列
                return ThreadUtils.getShardParam();
            }
        }

        // 如果针对该列进行散表，则必须包含该列作为查询条件。
        throw new SQLSyntaxErrorException("SQL [" + parseInfo.getSQL()
                + "] Query without shard parameter: " // NL
                + column.getSql());
    }

    @Override
    protected boolean analyzeParam(int parameterIndex, Object x, int sqlType) throws SQLException {

        if (logger.isDebugEnabled()) {
            logger.debug("Invoking analyzing Param: [" + parameterIndex + "] "
                    + JdbcUtils.getTypeName(sqlType) + ", " + x);
        }

        // 检查参数索引的数据列 
        Column paramColumn = parseInfo.getColumn(parameterIndex);

        if (paramColumn != null) {

            // 检查是否为散表的列
            if ((partitionColumn != null) && partitionColumn.equals(paramColumn)) {

                partition = partitionRouter.doRoute(x);
            }

            // 检查是否为散库的列
            if ((dbColumn != null) && dbColumn.equals(paramColumn)) {

                String dbPattern = dbRouter.doRoute(x);

                // 检查状态参数是否变更 
                if (changeConnection(dbPattern)) {

                    freeConnection(); // 切换使用的连接

                    this.dbPattern = dbPattern;

                    return true;
                }
            }
        }

        return false;
    }

    @Override
    protected String rewriteSql(String sql) {

        if (logger.isDebugEnabled()) {
            logger.debug("Invoking rewriting: " + sql);
        }

        // 需要替换的数据表。
        if ((partition != null) && !partition.equals(name)) {

            // 使用  SqlRewriter 拆分语句，进行所需的查找和替换。
            String sqlRewrited = SqlRewriter.rewriteSqlTable(sql, name, partition);

            // 输出重写日志
            if (logger.isDebugEnabled()) {
                logger.debug("Rewriting SQL: \n  From: " + sql + "\n  To:   " + sqlRewrited);
            }

            return sqlRewrited;
        }

        return sql;
    }

    @Override
    protected Connection getConnection() throws SQLException {

        Connection connect;

        if (!desireRead) {

            // 获取可写数据库连接。
            connect = ds.getWriteConnection(dbName, dbPattern);

        } else {

            // 获取只读数据库连接。
            connect = ds.getReadConnection(dbName, dbPattern);
        }

        if (connect == null) {

            // 抛出数据库连接错误，并显示当前的查询语句
            StringBuilder builder = new StringBuilder("Connection not found: ");
            builder.append(dbName);

            if (dbPattern != null) {
                builder.append(".");
                builder.append(dbPattern);
            }

            builder.append('[');
            builder.append(desireRead ? "slave" : "master");
            builder.append(']');

            if (parseInfo != null) {
                builder.append(", SQL [");
                builder.append(parseInfo.getSQL());
                builder.append(']');
            }

            throw new SQLNonTransientConnectionException(builder.toString());
        }

        return connect;
    }
}
