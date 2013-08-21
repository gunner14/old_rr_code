package com.xiaonei.commons.jade.impl.xceadapter.provider;

import java.io.File;

import javax.sql.DataSource;

import net.paoding.rose.jade.jadeinterface.app.ResourceUtils;
import net.paoding.rose.jade.jadeinterface.datasource.DataSourceFactory;
import net.paoding.rose.jade.jadeinterface.provider.DataAccess;
import net.paoding.rose.jade.jadeinterface.provider.exql.ExqlDataAccessProvider;

import org.springframework.core.io.DefaultResourceLoader;
import org.springframework.core.io.Resource;
import org.springframework.core.io.ResourceLoader;

import com.xiaonei.commons.jade.impl.xceadapter.configurator.XmlDocXceConfigurator;
import com.xiaonei.commons.jade.impl.xceadapter.connector.LocalXceConnector;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceDataSourceFactory;

/**
 * 提供线上 xceadapter 版本的
 * {@link net.paoding.rose.jade.jadeinterface.provider.DataAccessProvider}
 * 实现。
 * 
 * @author han.liao
 */
public class LocalDataAccessProvider extends ExqlDataAccessProvider {

    private String location;

    public LocalDataAccessProvider() {
        super();
    }

    public LocalDataAccessProvider(String location) {
        this.location = location;
    }

    public LocalDataAccessProvider(File file) {
        this.location = file.toURI().toString();
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public void setFile(File file) {
        this.location = file.toURI().toString();
    }

    public String getLocation() {
        return location;
    }

    @Override
    public DataAccess createDataAccess(DataSource dataSource) {

        DataAccess dataAccess = super.createDataAccess(dataSource);
        return new XceDataAccess(dataAccess);
    }

    @Override
    protected DataSourceFactory createDataSourceFactory() {

        // 获取配置路径
        Resource configResource;
        if (location != null) {
            // 如果配置了文件路径
            if (applicationContext instanceof ResourceLoader) {
                configResource = ((ResourceLoader) applicationContext).getResource(location);
            } else {
                configResource = (new DefaultResourceLoader()).getResource(location);
            }
        } else {
            // 如果没有配置文件路径
            configResource = ResourceUtils.findResource(applicationContext, // NL
                    "jade-jdbc.properties");
        }

        // 创建本地连接器
        LocalXceConnector connector = new LocalXceConnector();
        connector.setLocations(new Resource[] { configResource });

        // 创建数据源工厂
        XceDataSourceFactory dataSourceFactory = new XceDataSourceFactory();
        dataSourceFactory.setConfigurator(new XmlDocXceConfigurator());
        dataSourceFactory.setConnector(connector);

        return dataSourceFactory;
    }
}
