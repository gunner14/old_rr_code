package com.xiaonei.commons.jade.impl.xceadapter.connector;

import java.sql.Connection;
import java.sql.SQLException;

import org.apache.commons.dbcp.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceConnector;

/**
 * 测试的: XceConnector 连接器, 没有实际功能。
 * 
 * @author han.liao
 */
public class TestXceConnector implements XceConnector {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(TestXceConnector.class);

    protected BasicDataSource ds = new BasicDataSource();

    /**
     * 创建: TestXceConnector 连接器。
     */
    public TestXceConnector() {

        try {
            Class.forName("com.mysql.jdbc.Driver");

        } catch (ClassNotFoundException e) {

            logger.error("Class not found: com.mysql.jdbc.Driver", e);
        }

        ds.setDriverClassName("com.mysql.jdbc.Driver");

        ds.setUrl("jdbc:mysql://localhost/test?useUnicode=true&characterEncoding=utf8");

        ds.setUsername("root");
        ds.setPassword("root");

        ds.setMinIdle(8);
        ds.setMaxIdle(16);
        ds.setMaxActive(32);
    }

    /**
     * 返回: DriverClassName 属性。
     * 
     * @return DriverClassName 属性
     */
    public String getDriverClassName() {

        return ds.getDriverClassName();
    }

    /**
     * 设置: DriverClassName 属性。
     * 
     * @param driverClassName - DriverClassName 属性
     */
    public void setDriverClassName(String driverClassName) {

        try {
            Class.forName(driverClassName);

        } catch (ClassNotFoundException e) {

            logger.error("Class not found: " + driverClassName, e);
        }

        ds.setDriverClassName(driverClassName);
    }

    /**
     * 返回数据库连接网址。
     * 
     * @return 数据库连接网址
     */
    public String getUrl() {
        return ds.getUrl();
    }

    /**
     * 设置数据库连接网址。
     * 
     * @param url - 数据库连接网址
     */
    public void setUrl(String url) {
        ds.setUrl(url);
    }

    /**
     * 返回数据库用户名。
     * 
     * @return 数据库用户名
     */
    public String getUsername() {
        return ds.getUsername();
    }

    /**
     * 设置数据库用户名。
     * 
     * @param username - 数据库用户名
     */
    public void setUsername(String username) {
        ds.setUsername(username);
    }

    /**
     * 返回数据库密码。
     * 
     * @return 数据库密码
     */
    public String getPassword() {
        return ds.getPassword();
    }

    /**
     * 设置数据库密码。
     * 
     * @param password - 数据库密码
     */
    public void setPassword(String password) {
        ds.setPassword(password);
    }

    @Override
    public Connection getReadConnection(String bizName, String bizRoute) throws SQLException {

        if (bizRoute == null) {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting read connection [" + bizName + ']');
            }

        } else {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting read connection [" + bizName + '.' + bizRoute + ']');
            }
        }

        return ds.getConnection();
    }

    @Override
    public Connection getWriteConnection(String bizName, String bizRoute) throws SQLException {

        if (bizRoute == null) {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting write connection [" + bizName + ']');
            }

        } else {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting write connection [" + bizName + '.' + bizRoute + ']');
            }
        }

        return ds.getConnection();
    }
}
