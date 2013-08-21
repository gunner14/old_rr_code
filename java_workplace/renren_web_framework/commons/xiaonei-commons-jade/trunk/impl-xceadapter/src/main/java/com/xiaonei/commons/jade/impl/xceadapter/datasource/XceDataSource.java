package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.sql.Connection;
import java.sql.SQLException;

import org.apache.commons.collections.map.LRUMap;

import com.xiaonei.commons.jade.impl.jdbcwrapper.HADataSource;
import com.xiaonei.commons.jade.impl.xceadapter.connector.DefaultXceConnector;

/**
 * 实现依赖 : XceAdapter / BizName 的数据源。
 * 
 * @author han.liao
 */
public class XceDataSource extends HADataSource {

    // 缓存语句的解析信息
    protected LRUMap cacheInfo = new LRUMap(256); // TODO: [NEED-CODE] 大小需要配置

    // 配置信息
    protected XceConfigurator configurator;

    // 使用的连接器
    protected XceConnector connector = DefaultXceConnector.getInstance();

    // 业务模块的名称 
    protected String catalog = "";

    // 数据源是否只读
    protected boolean readOnly;

    // 强制访问写数据库
    protected boolean forceWrite;

    /**
     * 默认的构造函数。
     */
    public XceDataSource() {
        super();
    }

    /**
     * 指定配置信息的构造函数。
     * 
     * @param configurator - 配置信息
     */
    public XceDataSource(XceConfigurator configurator) {

        this.configurator = configurator;
    }

    /**
     * 指定配置信息与连接器的构造函数。
     * 
     * @param configurator - 配置信息
     * 
     * @param connector - 指定的连接器
     */
    public XceDataSource(XceConfigurator configurator, XceConnector connector) {

        this.configurator = configurator;

        this.connector = connector;
    }

    /**
     * 返回配置信息。
     * 
     * @return 配置信息
     */
    public XceConfigurator getConfigurator() {
        return configurator;
    }

    /**
     * 设置配置信息。
     * 
     * @param configurator - 配置信息
     */
    public void setConfigurator(XceConfigurator configurator) {
        this.configurator = configurator;
    }

    /**
     * 返回使用的连接器。
     * 
     * @return 使用的连接器
     */
    public XceConnector getConnector() {
        return connector;
    }

    /**
     * 设置使用的连接器。
     * 
     * @param connector - 使用的连接器
     */
    public void setConnector(XceConnector connector) {
        this.connector = connector;
    }

    /**
     * 返回业务模块的名称 。
     * 
     * @return 业务模块的名称
     */
    public String getCatalog() {
        return catalog;
    }

    /**
     * 设置业务模块的名称 。
     * 
     * @param catalog - 业务模块的名称
     */
    public void setCatalog(String catalog) {
        this.catalog = catalog;
    }

    /**
     * 检查数据源是否只读。
     * 
     * @return 数据源是否只读
     */
    public boolean isReadOnly() {
        return readOnly;
    }

    /**
     * 设置数据源是否只读。
     * 
     * @param readOnly - 数据源是否只读
     */
    public void setReadOnly(boolean readOnly) {
        this.readOnly = readOnly;
    }

    /**
     * 检查数据源是否强制访问写。
     * 
     * @return 数据源是否强制访问写
     */
    public boolean isForceWrite() {
        return forceWrite;
    }

    /**
     * 设置数据源是否强制访问写。
     * 
     * @param forceWrite - 数据源是否强制访问写
     */
    public void setForceWrite(boolean forceWrite) {
        this.forceWrite = forceWrite;
    }

    @Override
    public Connection getConnection() throws SQLException {

        return new XceConnection(this);
    }

    /**
     * 返回 Catalog 默认的路由配置。
     * 
     * @return 路由配置
     */
    protected XceDescriptor getCatalogDescriptor() {

        return configurator.getCatalogDescriptor(catalog);
    }

    /**
     * 返回指定表名的路由配置。
     * 
     * @param name - 指定的表名
     * 
     * @return 路由配置
     */
    protected XceDescriptor getDescriptor(String name) {

        return configurator.getDescriptor(catalog, name);
    }

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
    protected Connection getWriteConnection(String bizName, String bizRoute) throws SQLException {

        return connector.getWriteConnection(bizName, bizRoute);
    }

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
    protected Connection getReadConnection(String bizName, String bizRoute) throws SQLException {

        return connector.getReadConnection(bizName, bizRoute);
    }

    /**
     * 从指定的语句获取解析信息。
     * 
     * @param sql - 指定的语句
     * 
     * @return XceParseInfo 对象
     */
    protected XceParseInfo getParseInfo(String sql) {

        XceParseInfo info;

        // 进入全局锁创建共享信息对象。
        synchronized (cacheInfo) {

            info = (XceParseInfo) cacheInfo.get(sql);

            if (info == null) {

                // 创建  XceParseInfo 实例是一个不耗时的操作。
                info = new XceParseInfo(sql);
                cacheInfo.put(sql, info);
            }
        }

        // 下面进行耗时操作，单独锁定记录本身。
        synchronized (info) {

            if (!info.isInited()) {

                // 输出日志
                if (logger.isDebugEnabled()) {
                    logger.debug("First parsing SQL: " + sql);
                }

                // 解析所给的语句, 也许比较耗时。
                info.parseInfo();
            }
        }

        return info;
    }
}
