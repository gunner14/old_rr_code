package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.sql.Connection;
import java.sql.SQLException;

import javax.sql.DataSource;

/**
 * 用于测试的简单数据库连接, 没有实际功能。
 * 
 * @author han.liao
 */
public class SimpleHAConnection extends HAConnection {

    protected DataSource ds;

    /**
     * 创建: SimpleHADataSource 实例。
     * 
     * @param source - 指定的数据源。
     */
    public SimpleHAConnection(DataSource ds) {

        this.ds = ds;
    }

    @Override
    protected Connection getConnection() throws SQLException {

        return ds.getConnection();
    }
}
