package com.xiaonei.commons.jade.impl.xceadapter.connector;

import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import javax.sql.DataSource;

import org.apache.commons.dbcp.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.core.io.Resource;

import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceConnector;

public class LocalXceConnector implements XceConnector {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(LocalXceConnector.class);

    // 缓存全部的: DataSource 对象
    protected ConcurrentHashMap<String, DataSource> map = new ConcurrentHashMap<String, DataSource>();

    // 配置信息
    protected ConcurrentHashMap<String, String> properties = new ConcurrentHashMap<String, String>();

    // 配置文件位置
    protected Resource[] locations;

    /**
     * 返回配置文件位置。
     * 
     * @return 配置文件位置
     */
    public Resource[] getLocations() {

        return locations;
    }

    /**
     * 设置配置文件位置。
     * 
     * @param locations - 配置文件位置
     */
    public void setLocations(Resource[] locations) {

        // 加载配置文件
        loadConfiguration(locations);

        this.locations = locations;
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

        DataSource ds = getDataSource(bizName, bizRoute);

        if (ds != null) {

            Connection conn = ds.getConnection();

            conn.setReadOnly(true);

            return conn;
        }

        return null;
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

        DataSource ds = getDataSource(bizName, bizRoute);

        if (ds != null) {

            return ds.getConnection();
        }

        return null;
    }

    /**
     * 从配置中获取数据源。
     * 
     * @param bizName - 业务模块的名称
     * 
     * @param bizRoute - 业务模块的后缀，可能为 <code>null</code>
     * 
     * @return 数据源
     */
    protected DataSource getDataSource(String bizName, String bizRoute) {

        String name = bizName;

        // 业务模块名称.业务模块后缀
        if (bizRoute != null) {
            name = bizName + '.' + bizRoute;
        }

        DataSource ds = doGetDataSource(name);

        if (ds == null) {

            if (bizRoute != null) {
                // 业务模块名称
                ds = doGetDataSource(bizName);
            }

            if (ds == null) {
                // 默认使用的数据源
                ds = doGetDataSource("jade-jdbc");
            }
        }

        return ds;
    }

    /**
     * 从配置中获取数据源。
     * 
     * @param name - 数据源的名称
     * 
     * @return 数据源
     */
    private DataSource doGetDataSource(String name) {

        DataSource ds = map.get(name);

        if (ds == null) {

            // 取得配置信息
            String url = properties.get(name + ".url");
            String driver = properties.get(name + ".driver");
            String username = properties.get(name + ".username");
            String password = properties.get(name + ".password");

            if (url != null) {

                // 默认使用  MySQL 驱动
                if (driver == null) {
                    driver = "com.mysql.jdbc.Driver";
                }

                if (username == null) {

                    // 输出日志
                    if (logger.isWarnEnabled()) {
                        logger.warn("Property \'" + name + ".username\' must set");
                    }
                }

                if (password == null) {

                    // 输出日志
                    if (logger.isWarnEnabled()) {
                        logger.warn("Property \'" + name + ".password\' must set");
                    }
                }

                // 创建数据源
                BasicDataSource bds = new BasicDataSource();

                bds.setUrl(url);
                bds.setUsername(username);
                bds.setPassword(password);
                bds.setDriverClassName(driver);

                map.putIfAbsent(name, bds);

                // 输出日志
                if (logger.isDebugEnabled()) {
                    logger.debug("DataSource \'" + name + "\' found");
                }

                return bds;
            }

            // 输出日志
            if (logger.isDebugEnabled()) {
                logger.debug("DataSource \'" + name + "\' not found");
            }
        }

        return ds;
    }

    /**
     * 从指定的配置文件位置读取配置。
     * 
     * @param locations - 指定的配置文件位置
     */
    protected void loadConfiguration(Resource[] locations) {

        // 替换原有配置信息
        properties.clear();

        for (Resource location : locations) {

            try {
                // 加载配置信息
                Properties props = loadProperties(location);

                putAllProps(properties, props);

                // 输出日志
                if (logger.isDebugEnabled()) {
                    logger.debug("Loading [" + location.getDescription() + ']');
                }

            } catch (IOException e) {

                // 输出日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Can't load [" + location.getDescription() + ']', e);
                }
            }
        }

        // 清理缓存
        map.clear();
    }

    /**
     * 将: Properties 集合并入: Map<String, String> 集合。
     * 
     * @param props - Properties 集合
     * 
     * @return Map<String, String> 集合
     */
    protected static void putAllProps(Map<String, String> map, Properties props) {

        for (Entry<Object, Object> entry : props.entrySet()) {

            // 如果有类型不符合的对象，请抛出异常
            map.put((String) entry.getKey(), (String) entry.getValue());
        }
    }

    /**
     * 从指定的文件位置读取配置。
     * 
     * @param location - 指定的文件位置
     */
    protected static Properties loadProperties(Resource location) throws IOException {

        InputStreamReader in = null;

        try {
            // 打开文件流
            in = new InputStreamReader(location.getInputStream(), "UTF-8");

            // 加载配置信息
            Properties props = new Properties();

            props.load(in);

            return props;

        } finally {

            // 关闭文件流
            if (in != null) {

                try {
                    in.close();

                } catch (IOException e) {

                    // 输出日志
                    if (logger.isWarnEnabled()) {
                        logger.warn("Can't close stream", e);
                    }
                }
            }
        }
    }
}
