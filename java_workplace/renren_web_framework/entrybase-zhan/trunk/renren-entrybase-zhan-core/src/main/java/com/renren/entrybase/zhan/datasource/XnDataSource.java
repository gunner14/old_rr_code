package com.renren.entrybase.zhan.datasource;

import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.Map;

import javax.sql.DataSource;

import net.paoding.rose.jade.annotation.SQLType;
import net.paoding.rose.jade.annotation.UseMaster;
import net.paoding.rose.jade.statement.StatementMetaData;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.entrybase.mysql.dao.supports.PartitionSQLInterpreter;
import com.xiaonei.xce.XceAdapter;
import com.xiaonei.xce.pool.DataSourceInstanceIF;

/**
 * 
 * @author 王志亮 [qieqie.wang@gmail.com]
 * @author 廖涵 [in355hz@gmail.com]
 * 
 */
public class XnDataSource implements DataSource {

    protected final Log logger = LogFactory.getLog(XnDataSource.class);

    public static final String DB_PATTERN = XnDataSource.class.getName() + "#DB_PATTERN";

    public static final int SHARD_MOD = 100;
    
    private String catalog;

    private StatementMetaData metaData;

    private Map<String, Object> runtimeProperties;

    public XnDataSource(String catalog, StatementMetaData metaData,
            Map<String, Object> runtimeProperties) {
        this.catalog = catalog;
        this.metaData = metaData;
        this.runtimeProperties = runtimeProperties;
    }

    public void setCatalog(String catalog) {
        this.catalog = catalog;
    }

    public String getCatalog() {
        return catalog;
    }

    public Connection getConnection() throws SQLException {
        boolean write = false;
        if (metaData.getSQLType() == SQLType.WRITE) {
            write = true;
        } else if (metaData.getMethod().isAnnotationPresent(UseMaster.class)) {
            write = true;
        }
        //String pattern = (String) runtimeProperties.get(DB_PATTERN);
        String pattern = null;
        Object shardValue = runtimeProperties.get(PartitionSQLInterpreter.SHARD_VALUE);
        String tableName = (String)runtimeProperties.get(PartitionSQLInterpreter.TABLE);
        if (shardValue instanceof Long && tableName != null && !tableName.startsWith("index_create_time")) {
            pattern = catalog + "_" + (((Long)shardValue) % SHARD_MOD);
        }
        if (pattern == null) {
            if (logger.isDebugEnabled()) {
                logger.debug("not found DB_PATTERN, using default pattern '' for SQL '"
                        + metaData.getSQL() + "'");
            }
            pattern = DataSourceInstanceIF.EMPTY_PATTERN;
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("found DB_PATTERN='" + pattern + "' for SQL '" + metaData.getSQL()
                        + "'");
            }
        }
        Connection conn;
        if (write) {
            conn = XceAdapter.getInstance().getWriteConnection(catalog, pattern);
        } else {
            conn = XceAdapter.getInstance().getReadConnection(catalog, pattern);
        }
        if (conn == null) {
            throw new SQLException("could't get " + (write ? "Write" : "Read")
                    + " connection from catalog '" + catalog + "' for pattern '" + pattern + "'");
        }
        return conn;
    }

    public Connection getConnection(String username, String password) throws SQLException {
        return getConnection();
    }

    //---------------------------------------

    public PrintWriter getLogWriter() throws SQLException {
        return null;
    }

    public int getLoginTimeout() throws SQLException {
        return 0;
    }

    public void setLogWriter(PrintWriter out) throws SQLException {

    }

    public void setLoginTimeout(int seconds) throws SQLException {

    }

    @SuppressWarnings({ "rawtypes" })
    public boolean isWrapperFor(Class arg0) throws SQLException {
        return false;
    }

    @SuppressWarnings({ "rawtypes", "unchecked" })
    public Object unwrap(Class arg0) throws SQLException {
        return null;
    }

    @Override
    public String toString() {
        return String.format("dataSource[bizName=%s]", catalog);
    }

    public void close() {
    }
}

