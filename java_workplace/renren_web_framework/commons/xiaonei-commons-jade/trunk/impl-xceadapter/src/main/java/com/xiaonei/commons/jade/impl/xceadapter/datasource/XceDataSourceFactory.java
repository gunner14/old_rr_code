package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.util.concurrent.ConcurrentHashMap;

import javax.sql.DataSource;

import net.paoding.rose.jade.jadeinterface.datasource.DataSourceFactory;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.jade.impl.xceadapter.connector.DefaultXceConnector;

/**
 * 实现: XceDataSource 的工厂。
 * 
 * @author han.liao
 */
public class XceDataSourceFactory implements DataSourceFactory {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(XceDataSourceFactory.class);

    // 缓存全部的: XceDataSource 对象
    protected ConcurrentHashMap<String, XceDataSource> map = new ConcurrentHashMap<String, XceDataSource>();

    // 配置信息
    protected XceConfigurator configurator;

    // 使用的连接器
    protected XceConnector connector = DefaultXceConnector.getInstance();

    /**
     * 默认的构造函数。
     */
    public XceDataSourceFactory() {

        super();
    }

    /**
     * 指定配置信息的构造函数。
     * 
     * @param configurator - 配置信息
     */
    public XceDataSourceFactory(XceConfigurator configurator) {

        this.configurator = configurator;
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
     * 从指定的: Catalog 获取: DataSource 数据源。
     * 
     * @param catalog - 指定的: Catalog
     * 
     * @return DataSource 数据源
     */
    @Override
    public DataSource getDataSource(String catalog) {

        if (catalog == null) {
            throw new IllegalArgumentException("Arg 'catalog' must not be null");
        }

        XceDataSource ds = map.get(catalog);

        if (ds == null) {

            // 创建  XceDataSource 数据源。
            ds = new XceDataSource(configurator, connector);

            ds.setCatalog(catalog);

            // putIfAbsent 绑定效率比较高
            map.putIfAbsent(catalog, ds);
        }

        return ds;
    }
}
