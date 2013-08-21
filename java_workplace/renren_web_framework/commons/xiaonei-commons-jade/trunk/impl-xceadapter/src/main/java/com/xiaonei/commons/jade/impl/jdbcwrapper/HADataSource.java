package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.sql.Connection;
import java.sql.SQLException;

/**
 * 实现可扩展数据库访问策略的数据源。
 * 
 * @author han.liao
 */
public class HADataSource extends BaseDataSource {

    @Override
    public Connection getConnection() throws SQLException {

        // TODO: [NEED-FIX] 这段代码永远不会执行到。

        if (logger.isWarnEnabled()) {
            logger.warn("Returning disabled superclass [HAConnection]");
        }

        return new HAConnection();
    }

    @Override
    public Connection getConnection(String username, String password) throws SQLException {

        return getConnection();
    }
}
