package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.sql.Connection;
import java.sql.SQLException;

/**
 * 定义获得数据库连接的接口, 便于进行测试。
 * 
 * @author han.liao
 */
public interface XceConnector {

    /**
     * 返回写入的数据库连接。
     * 
     * @param bizName - 业务模块的名称
     * 
     * @param bizRoute - 业务模块的后缀，可能为 <code>null</code>
     * 
     * @return 写入的数据库连接
     * 
     * @throws SQLException
     */
    Connection getWriteConnection(String bizName, String bizRoute) throws SQLException;

    /**
     * 返回只读的数据库连接。
     * 
     * @param bizName - 业务模块的名称
     * 
     * @param bizRoute - 业务模块的后缀，可能为 <code>null</code>
     * 
     * @return 只读的数据库连接
     * 
     * @throws SQLException
     */
    Connection getReadConnection(String bizName, String bizRoute) throws SQLException;
}
