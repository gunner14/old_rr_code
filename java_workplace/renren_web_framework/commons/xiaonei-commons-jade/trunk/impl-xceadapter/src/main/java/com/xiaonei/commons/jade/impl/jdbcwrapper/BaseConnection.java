package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.sql.Array;
import java.sql.Blob;
import java.sql.CallableStatement;
import java.sql.Clob;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.NClob;
import java.sql.PreparedStatement;
import java.sql.SQLClientInfoException;
import java.sql.SQLException;
import java.sql.SQLWarning;
import java.sql.SQLXML;
import java.sql.Savepoint;
import java.sql.Statement;
import java.sql.Struct;
import java.util.Map;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 继承: BaseConnection 定制数据库连接。
 * 
 * <P>
 * BaseConnection 将真实: Connection 的获取推迟到功能函数调用。
 * 
 * @author han.liao
 */
public abstract class BaseConnection implements Connection {

    /** 供派生类使用 */
    protected static final Log logger = LogFactory.getLog(BaseConnection.class);

    // 内部的数据库连接。
    protected Connection connect;

    protected boolean autoCommit = true, readOnly = false;

    protected String catalog;

    protected Properties clientInfo;

    protected int holdability = -1, isolationLevel = -1;

    protected Map<String, Class<?>> mapOfType;

    protected boolean closed; // 关闭标志

    @Override
    public void close() throws SQLException {

        freeConnection();

        closed = true;
    }

    @Override
    public void commit() throws SQLException {

        initConnection();

        connect.commit();
    }

    @Override
    public Array createArrayOf(String typeName, Object[] elements) throws SQLException {

        initConnection();

        return connect.createArrayOf(typeName, elements);
    }

    @Override
    public Blob createBlob() throws SQLException {

        initConnection();

        return connect.createBlob();
    }

    @Override
    public Clob createClob() throws SQLException {

        initConnection();

        return connect.createClob();
    }

    @Override
    public NClob createNClob() throws SQLException {

        initConnection();

        return connect.createNClob();
    }

    @Override
    public SQLXML createSQLXML() throws SQLException {

        initConnection();

        return connect.createSQLXML();
    }

    @Override
    public Statement createStatement() throws SQLException {

        initConnection();

        return connect.createStatement();
    }

    @Override
    public Statement createStatement(int resultSetType, int resultSetConcurrency)
            throws SQLException {

        initConnection();

        return connect.createStatement(resultSetType, resultSetConcurrency);
    }

    @Override
    public Statement createStatement(int resultSetType, int resultSetConcurrency,
            int resultSetHoldability) throws SQLException {

        initConnection();

        return connect.createStatement(resultSetType, resultSetConcurrency, resultSetHoldability);
    }

    @Override
    public Struct createStruct(String typeName, Object[] attributes) throws SQLException {

        initConnection();

        return connect.createStruct(typeName, attributes);
    }

    @Override
    public boolean getAutoCommit() throws SQLException {

        if (connect != null) {
            return connect.getAutoCommit();
        }

        return autoCommit;
    }

    @Override
    public void setAutoCommit(boolean autoCommit) throws SQLException {

        if (connect != null) {
            connect.setAutoCommit(autoCommit);
        }

        this.autoCommit = autoCommit;
    }

    @Override
    public String getCatalog() throws SQLException {

        initConnection();

        return connect.getCatalog();
    }

    @Override
    public void setCatalog(String catalog) throws SQLException {

        if (connect != null) {
            connect.setCatalog(catalog);
        }

        this.catalog = catalog;
    }

    @Override
    public Properties getClientInfo() throws SQLException {

        initConnection();

        return connect.getClientInfo();
    }

    @Override
    public String getClientInfo(String name) throws SQLException {

        initConnection();

        return connect.getClientInfo(name);
    }

    @Override
    public void setClientInfo(Properties properties) throws SQLClientInfoException {

        if (connect != null) {
            connect.setClientInfo(properties);
        }

        clientInfo = properties;
    }

    @Override
    public void setClientInfo(String name, String value) throws SQLClientInfoException {

        if (connect != null) {
            connect.setClientInfo(name, value);
        }

        if (clientInfo == null) {
            clientInfo = new Properties();
            clientInfo.setProperty(name, value);
        }
    }

    @Override
    public int getHoldability() throws SQLException {

        initConnection();

        return connect.getHoldability();
    }

    @Override
    public void setHoldability(int holdability) throws SQLException {

        if (connect != null) {
            connect.setHoldability(holdability);
        }

        this.holdability = holdability;
    }

    @Override
    public int getTransactionIsolation() throws SQLException {

        initConnection();

        return connect.getTransactionIsolation();
    }

    @Override
    public void setTransactionIsolation(int level) throws SQLException {

        if (connect != null) {
            connect.setTransactionIsolation(isolationLevel);
        }

        isolationLevel = level;
    }

    @Override
    public boolean isReadOnly() throws SQLException {

        if (connect != null) {
            return connect.isReadOnly();
        }

        return readOnly;
    }

    @Override
    public void setReadOnly(boolean readOnly) throws SQLException {

        if (connect != null) {
            connect.setReadOnly(readOnly);
        }

        this.readOnly = readOnly;
    }

    @Override
    public Map<String, Class<?>> getTypeMap() throws SQLException {

        initConnection();

        return connect.getTypeMap();
    }

    @Override
    public void setTypeMap(Map<String, Class<?>> map) throws SQLException {

        if (connect != null) {
            connect.setTypeMap(map);
        }

        mapOfType = map;
    }

    @Override
    public DatabaseMetaData getMetaData() throws SQLException {

        initConnection();

        return connect.getMetaData();
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {

        initConnection();

        return connect.getWarnings();
    }

    @Override
    public void clearWarnings() throws SQLException {

        if (connect != null) {
            connect.clearWarnings();
        }
    }

    @Override
    public boolean isValid(int timeout) throws SQLException {

        if (connect != null) {
            return connect.isValid(timeout);
        }

        return !closed;
    }

    @Override
    public boolean isClosed() throws SQLException {

        if (connect != null) {
            return connect.isClosed();
        }

        return closed;
    }

    @Override
    public String nativeSQL(String sql) throws SQLException {

        initConnection();

        return connect.nativeSQL(sql);
    }

    @Override
    public CallableStatement prepareCall(String sql) throws SQLException {

        initConnection();

        return connect.prepareCall(sql);
    }

    @Override
    public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency)
            throws SQLException {

        initConnection();

        return connect.prepareCall(sql, resultSetType, resultSetConcurrency);
    }

    @Override
    public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency,
            int resultSetHoldability) throws SQLException {

        initConnection();

        return connect.prepareCall(sql, resultSetType, resultSetConcurrency, resultSetHoldability);
    }

    @Override
    public PreparedStatement prepareStatement(String sql) throws SQLException {

        initConnection();

        return connect.prepareStatement(sql);
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int autoGeneratedKeys)
            throws SQLException {

        initConnection();

        return connect.prepareStatement(sql, autoGeneratedKeys);
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int[] columnIndexes) throws SQLException {

        initConnection();

        return connect.prepareStatement(sql, columnIndexes);
    }

    @Override
    public PreparedStatement prepareStatement(String sql, String[] columnNames) throws SQLException {

        initConnection();

        return connect.prepareStatement(sql, columnNames);
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int resultSetType,
            int resultSetConcurrency) throws SQLException {

        initConnection();

        return connect.prepareStatement(sql, resultSetType, resultSetConcurrency);
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int resultSetType,
            int resultSetConcurrency, int resultSetHoldability) throws SQLException {

        initConnection();

        return connect.prepareStatement(sql, resultSetType, resultSetConcurrency,
                resultSetHoldability);
    }

    @Override
    public void rollback() throws SQLException {

        initConnection();

        connect.rollback();
    }

    @Override
    public void rollback(Savepoint savepoint) throws SQLException {

        initConnection();

        connect.rollback(savepoint);
    }

    @Override
    public Savepoint setSavepoint() throws SQLException {

        initConnection();

        return connect.setSavepoint();
    }

    @Override
    public Savepoint setSavepoint(String name) throws SQLException {

        initConnection();

        return connect.setSavepoint(name);
    }

    @Override
    public void releaseSavepoint(Savepoint savepoint) throws SQLException {

        initConnection();

        connect.releaseSavepoint(savepoint);
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        return Connection.class.equals(iface);
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {

        if (!Connection.class.equals(iface)) {
            throw new SQLException("Connection [" + getClass().getName()
                    + "] can't be unwrapped as [" + iface.getName() + "].");
        }

        return iface.cast(this);
    }

    /**
     * 实现该函数获取真实的数据库连接。 <br>
     * 
     * @return 真实的数据库连接。
     * 
     * @throws SQLException
     */
    protected abstract Connection getConnection() throws SQLException;

    /**
     * 如果没有就初始化数据库连接。
     * 
     * @throws SQLException
     */
    protected void initConnection() throws SQLException {

        if (connect == null) {

            if (closed) {
                throw new SQLException("Connection is closed.", // NL
                        "08003" /* SQLError.SQL_STATE_CONNECTION_NOT_OPEN */);
            }

            // 记录日志。
            if (logger.isDebugEnabled()) {
                logger.debug("Creating wrapped connection");
            }

            // 获取数据库连接。
            connect = getConnection();

            // 初始化数据库连接。
            connect.setAutoCommit(autoCommit);

            if (catalog != null) {
                connect.setCatalog(catalog);
            }

            if (clientInfo != null) {
                connect.setClientInfo(clientInfo);
            }

            if (holdability >= 0) {
                connect.setHoldability(holdability);
            }

            if (isolationLevel >= 0) {
                connect.setTransactionIsolation(isolationLevel);
            }

            if (mapOfType != null) {
                connect.setTypeMap(mapOfType);
            }

            connect.setReadOnly(readOnly);
        }
    }

    /**
     * 释放现在的数据库连接。
     * 
     * @throws SQLException
     */
    protected void freeConnection() throws SQLException {

        if (connect != null) {

            // 记录日志。
            if (logger.isDebugEnabled()) {
                logger.debug("Closing wrapped connection");
            }

            // 释放数据库连接。
            connect.close();
            connect = null;
        }
    }
}
