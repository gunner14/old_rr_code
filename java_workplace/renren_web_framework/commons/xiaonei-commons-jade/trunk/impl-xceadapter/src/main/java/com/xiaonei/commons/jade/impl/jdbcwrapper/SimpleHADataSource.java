package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.sql.Connection;
import java.sql.SQLException;

import javax.sql.DataSource;

/**
 * 用于测试的简单数据源, 没有实际功能。
 * 
 * @author han.liao
 */
public class SimpleHADataSource extends HADataSource {

    // 使用的内部数据源。
    protected DataSource ds;

    /**
     * 基本的构造函数。
     */
    public SimpleHADataSource() {
        super();
    }

    /**
     * 从使用的内部数据源构造。
     * 
     * @param dataSource - 内部数据源
     */
    public SimpleHADataSource(DataSource ds) {
        this.ds = ds;
    }

    /**
     * 返回使用的内部数据源。
     * 
     * @return 内部数据源
     */
    public DataSource getDataSource() {
        return ds;
    }

    /**
     * 设置使用的内部数据源。
     * 
     * @param ds - 内部数据源
     */
    public void setDataSource(DataSource ds) {
        this.ds = ds;
    }

    @Override
    public Connection getConnection() throws SQLException {

        return new SimpleHAConnection(ds);
    }
}
