package com.xiaonei.jade.datasource;

import java.util.Collections;
import java.util.Map;
import java.util.Random;

import javax.annotation.PostConstruct;
import javax.sql.DataSource;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.ShardParam;
import net.paoding.rose.jade.context.spring.SpringDataSourceFactory;
import net.paoding.rose.jade.core.Identity;
import net.paoding.rose.jade.statement.Interpreter;
import net.paoding.rose.jade.statement.StatementRuntime;
import net.paoding.rose.jade.statement.expression.ExprResolver;
import net.paoding.rose.jade.statement.expression.impl.ExprResolverImpl;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactoryUtils;
import org.springframework.beans.factory.NoSuchBeanDefinitionException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.core.annotation.Order;
import org.springframework.dao.InvalidDataAccessApiUsageException;
import org.springframework.jdbc.BadSqlGrammarException;
import org.springframework.jdbc.datasource.DelegatingDataSource;
import org.springframework.util.Assert;

import com.meidusa.amoeba.parser.dbobject.Column;
import com.meidusa.amoeba.parser.dbobject.Table;
import com.xiaonei.jade.datasource.configurator.RemoteXmlDocConfigurator;
import com.xiaonei.xce.pool.DataSourceInstanceIF;

// 按Spring语义规定，Order值越高该解释器越后执行
@Order(9000)
public class RouterInterpreter implements Interpreter, ApplicationContextAware {

    private static final Log logger = LogFactory.getLog(RouterInterpreter.class);

    private Map<String, Object> emptyProperties = Collections.emptyMap();

    private XnDataSourceFactory dataSourceFactory;

    // 配置信息
    protected RoutingConfigurator routingConfigurator;

    protected ApplicationContext applicationContext;

    public void setConfigurator(RoutingConfigurator routingConfigurator) {
        this.routingConfigurator = routingConfigurator;
    }

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }

    @PostConstruct
    public void initRoutingConfigurator() {
        if (this.routingConfigurator == null) {
            if (this.applicationContext != null) {
                try {
                    routingConfigurator = (RoutingConfigurator) BeanFactoryUtils.beanOfType(
                            applicationContext, RoutingConfigurator.class);
                } catch (NoSuchBeanDefinitionException e) {}
            }
        }
        if (this.routingConfigurator == null) {
            routingConfigurator = new RemoteXmlDocConfigurator();
        }
        this.dataSourceFactory = (XnDataSourceFactory) BeanFactoryUtils.beanOfType(
                applicationContext, XnDataSourceFactory.class);
    }

    @Override
    public void interpret(StatementRuntime runtime) {
        // 如果spring容器本身配置了对应的DataSource，且非XnDataSource，本解析器不做任何事情
        SpringDataSourceFactory spring = dataSourceFactory.getSpringDataSourceFactory();
        DataSource dataSource = spring.getDataSource(runtime.getMetaData(), emptyProperties);
        while (dataSource instanceof DelegatingDataSource) {
            dataSource = ((DelegatingDataSource) dataSource).getTargetDataSource();
        }
        String catalog;
        if (dataSource != null && !(dataSource instanceof XnDataSource)) {
            return;
        }
        //
        if (dataSource != null) {
            catalog = ((XnDataSource) dataSource).getCatalog();
        } else {
            Class<?> daoClass = runtime.getMetaData().getDAOMetaData().getDAOClass();
            catalog = daoClass.getAnnotation(DAO.class).catalog();
            if (catalog == null) {
                logger.debug("not found catalog for " + daoClass.getName());
                return;
            }
        }
        if (logger.isDebugEnabled()) {
            logger.debug("using catalog " + catalog + " for "
                    + runtime.getMetaData().getDAOMetaData());
        }

        // 确认执行此解析器前，已经经过了jade内置的标准解析器了（因此args将非空)
        Assert.notNull(runtime.getArgs(), "args must be not null before invoking this interpreter!");

        // 
        if (logger.isDebugEnabled()) {
            logger.debug("Invoking analyzing: " + runtime.getSQL());
        }

        SQLParseInfo parseInfo = SQLParseInfo.getParseInfo(runtime.getSQL());
        // 从查询的数据表获取路由配置。
        Table[] tables = parseInfo.getTables();
        RoutingInfo routingInfo = null;
        //
        if (tables != null) {
            int beginIndex = 0;
            if (parseInfo.isInsert() && tables.length > 1) {
                // INSERT ... SELECT 查询
                beginIndex = 1;
            }

            // 查找散表配置
            for (int i = beginIndex; i < tables.length; i++) {
                RoutingDescriptor descriptor = routingConfigurator.getDescriptor(//
                        catalog, tables[i].getName());
                if (descriptor != null) {
                    routingInfo = new RoutingInfo(tables[i], descriptor);
                    break;
                }
            }
        }
        if (routingInfo == null) {
            return;
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("Find routing info: " + routingInfo.byTable + ", "
                        + routingInfo.getDbRouterColumn());
            }
        }
        String forwardTableName = null;

        if (routingInfo.getTableRouter() != null) {

            // 用语句信息的常量进行散表。
            Column column = routingInfo.getTableRouterColumn();
            Object columnValue = null;

            if (column != null) {
                columnValue = findShardParamValue(runtime, parseInfo, column);
                if (columnValue == null) {
                    throw new BadSqlGrammarException("db routering", parseInfo.getSQL(), null);
                }
            }

            // 获得散表的名称
            forwardTableName = routingInfo.getTableRouter().doRoute(columnValue);
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("table router is null for sql \"" + parseInfo.getSQL() + "\"");
            }
        }

        // 散库
        if (routingInfo.getDbRouter() != null) {

            // 用语句信息的常量进行散库。
            Column column = routingInfo.getDbRouterColumn();
            Object columnValue = null;

            if (column != null) {
                columnValue = findShardParamValue(runtime, parseInfo, column);
                if (columnValue == null) {
                    throw new BadSqlGrammarException("table routering", parseInfo.getSQL(), null);
                }
            }

            // 获得散库的名称
            String forwardDbPattern = routingInfo.getDbRouter().doRoute(columnValue);
            if (forwardDbPattern != null) {
                if (logger.isDebugEnabled()) {
                    logger.debug("db pattern is '" + forwardDbPattern + "'");
                }
                runtime.setProperty(XnDataSource.DB_PATTERN, forwardDbPattern);
            } else {
                if (logger.isDebugEnabled()) {
                    logger.debug("db pattern is empty");
                }
                runtime.setProperty(XnDataSource.DB_PATTERN, DataSourceInstanceIF.EMPTY_PATTERN);
            }
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("no db router for sql \"" + runtime.getSQL() + "\"");
            }
        }
        // 散表
        String byTableName = routingInfo.byTable.getName();
        final String sqlRewrited;
        if (forwardTableName != null && !forwardTableName.equals(byTableName)) {

            // 使用  SqlRewriter 拆分语句，进行所需的查找和替换。
            sqlRewrited = SqlRewriter.rewriteSqlTable(parseInfo.getSQL(), byTableName,
                    forwardTableName);

            // 输出重写日志
            if (logger.isDebugEnabled()) {
                logger.debug("rewriting SQL: \n  From: " + parseInfo.getSQL() + "\n  To:   "
                        + sqlRewrited);
            }
            runtime.setSQL(sqlRewrited);
        }
    }

    class RoutingInfo {

        private Table byTable;

        private RoutingDescriptor descriptor;

        public RoutingInfo(Table table, RoutingDescriptor descriptor) {
            this.byTable = table;
            this.descriptor = descriptor;
        }

        public Router getDbRouter() {
            return descriptor.getDbRouter();
        }

        public Router getTableRouter() {
            return descriptor.getTableRouter();
        }

        private Column dbRouterColumn;

        public Column getDbRouterColumn() {
            if (dbRouterColumn != null) {
                return dbRouterColumn;
            }
            Router dbRouter = getDbRouter();
            if (dbRouter == null) {
                return null;
            }

            String columnName = dbRouter.getColumn();

            if (columnName != null) {

                // 保存匹配的数据列
                Column columnForDBPartition = new Column();
                columnForDBPartition.setName(columnName.toUpperCase());
                columnForDBPartition.setTable(byTable);
                this.dbRouterColumn = columnForDBPartition;
            }
            return dbRouterColumn;
        }

        private Column tableRouterColumn;

        public Column getTableRouterColumn() {
            if (tableRouterColumn != null) {
                return tableRouterColumn;
            }
            Router tableRouter = getTableRouter();
            if (tableRouter == null) {
                return null;
            }

            String columnName = tableRouter.getColumn();

            if (columnName != null) {

                // 保存匹配的数据列
                Column tableRouterColumn = new Column();
                tableRouterColumn.setName(columnName.toUpperCase());
                tableRouterColumn.setTable(byTable);
                this.tableRouterColumn = tableRouterColumn;
            }
            return tableRouterColumn;
        }
    }

    // 查找散表参数
    protected static Object findShardParamValue(StatementRuntime runtime, SQLParseInfo parseInfo,
            Column column) {
        int shardByIndex = runtime.getMetaData().getShardByIndex();
        Object value = null;
        if (shardByIndex >= 0) {
            value = runtime.getParameters().get(":" + (shardByIndex + 1));
            if (logger.isDebugEnabled()) {
                logger.debug("find shard param value '" + value + "' by @ShardBy at index "
                        + shardByIndex);
            }
            return value;
        } else {
            ShardParam shardParam = runtime.getMetaData().getMethod()
                    .getAnnotation(ShardParam.class);
            if (shardParam != null) {
                if (new Random().nextInt(50) == 1) {
                    new IllegalArgumentException(
                            "message by zhiliang.wang: please change @ShardParam to @ShardBy for "
                                    + runtime.getMetaData());
                }
                // 检查外部提供的散表参数
                Column shardColumn = SQLParseInfo.newColumn(shardParam.name());

                try {
                    ExprResolver exprResolver = new ExprResolverImpl(runtime.getParameters());
                    value = exprResolver.executeExpr(shardParam.value());

                } catch (Exception e) {
                    if (logger.isWarnEnabled()) {
                        logger.warn("Can't execute @ShardParam expr: " + shardParam.value(), e);
                    }
                    throw new InvalidDataAccessApiUsageException("Can't execute @ShardParam expr: "
                            + shardParam.value(), e);
                }

                if (ShardParam.WIDECARD.equals(shardColumn.getName())) {
                    // 匹配所有散表列
                    return value;
                } else if (shardColumn.getTable() == null) {
                    // 模糊的匹配散表列
                    if (shardColumn.getName().equals(column.getName())) {
                        return value;
                    }
                } else if (shardColumn.equals(column)) {
                    // 精确的匹配散表列
                    return value;
                }
                // 如果针对该列进行散表，则必须包含该列作为查询条件。
                throw new BadSqlGrammarException("interpreter.findShardParamValue@ShardParam",
                        "SQL [" + parseInfo.getSQL() + "] Query without shard parameter: " // NL
                                + column.getSql(), null);

            } else if (parseInfo.containsParam(column)) {
                // 获取语句中的散表参数
                value = parseInfo.getParam(column);
                // statement
                if (value != null) {
                    if (logger.isDebugEnabled()) {
                        logger.debug("find shard param value '" + value + "' by column "
                                + column.getName() + " [constants]");
                    }
                    return value;
                }
                // preparedStatment
                else {
                    int index = parseInfo.getColumnIndex(column);
                    Object[] args = runtime.getArgs();
                    if (index >= 0 && index < args.length) {
                        value = args[index];
                        if (logger.isDebugEnabled()) {
                            logger.debug("find shard param value '" + value + "' by column's arg "
                                    + column.getName() + " [index=" + index + " (beginwiths 0)]");
                        }
                    } else {
                        // 如果针对该列进行散表又没有设置@ShardBy注解的，则必须包含该列作为查询条件。
                        throw new BadSqlGrammarException("interpreter.findShardParamValue", "SQL ["
                                + parseInfo.getSQL() + "] Query without shard parameter: " // NL
                                + column.getSql(), null);
                    }
                    return value;
                }
            } else {
                // 如果针对该列进行散表，则必须包含该列作为查询条件。
                throw new BadSqlGrammarException("interpreter.findShardParamValue", "SQL ["
                        + parseInfo.getSQL() + "] Query without shard parameter: " // NL
                        + column.getSql(), null);
            }
        }
    }
}
