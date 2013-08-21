package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.sql.Connection;
import java.sql.SQLException;

import javax.sql.DataSource;

/**
 * 实现简单读写分离的数据源，用于功能验证。
 * 
 * @author han.liao
 */
public class RWSplitDataSource extends HADataSource {

    protected DataSource master, slaves;

    /**
     * 基本的构造函数。
     */
    public RWSplitDataSource() {

        super();
    }

    /**
     * 从使用的: Read-Write 分离数据源构造。
     * 
     * @param master - Writable 数据源
     * @param slaves - ReadOnly 数据源
     */
    public RWSplitDataSource(DataSource master, DataSource slaves) {

        this.master = master;
        this.slaves = slaves;
    }

    /**
     * 返回使用的写入数据源。
     * 
     * @return 写入数据源
     */
    public DataSource getWriteDataSource() {
        return master;
    }

    /**
     * 设置使用的写入数据源。
     * 
     * @param ds - 写入数据源
     */
    public void setWriteDataSource(DataSource ds) {
        this.master = ds;
    }

    /**
     * 返回使用的只读数据源。
     * 
     * @return 只读数据源
     */
    public DataSource getReadDataSource() {
        return slaves;
    }

    /**
     * 设置使用的只读数据源。
     * 
     * @param ds - 只读数据源
     */
    public void setReadDataSource(DataSource ds) {
        this.slaves = ds;
    }

    @Override
    public Connection getConnection() throws SQLException {

        return new RWSplitConnection(this);
    }

    /**
     * 返回写入的数据库连接。
     * 
     * @return 写入的数据库连接
     * 
     * @throws SQLException
     */
    protected Connection getWriteConnection() throws SQLException {

        Connection connect = master.getConnection();

        // 写入日志。
        if (logger.isInfoEnabled()) {
            logger.info("Get writable connection.");
        }

        return connect;
    }

    /**
     * 返回只读的数据库连接。
     * 
     * @return 只读的数据库连接
     * 
     * @throws SQLException
     */
    protected Connection getReadConnection() throws SQLException {

        Connection connect;

        try {
            connect = slaves.getConnection();

            // 写入日志。
            if (logger.isInfoEnabled()) {
                logger.info("Get read-only connection.");
            }

        } catch (SQLException e) {

            // 警告: 无法从只读数据源获得连接。
            if (logger.isWarnEnabled()) {
                logger.warn("Can't get read-only connection, using writable.", e);
            }

            // 使用写入数据源获得连接。
            connect = master.getConnection();
        }

        connect.setReadOnly(true);

        return connect;
    }
}
