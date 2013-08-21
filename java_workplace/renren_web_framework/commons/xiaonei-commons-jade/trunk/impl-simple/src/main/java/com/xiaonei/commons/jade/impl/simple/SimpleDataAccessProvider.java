package com.xiaonei.commons.jade.impl.simple;

import java.io.File;

import net.paoding.rose.jade.jadeinterface.app.ResourceUtils;
import net.paoding.rose.jade.jadeinterface.datasource.DataSourceFactory;
import net.paoding.rose.jade.jadeinterface.provider.exql.ExqlDataAccessProvider;

import org.springframework.core.io.DefaultResourceLoader;
import org.springframework.core.io.Resource;
import org.springframework.core.io.ResourceLoader;

public class SimpleDataAccessProvider extends ExqlDataAccessProvider {

    private String location;

    public SimpleDataAccessProvider() {
        super();
    }

    public SimpleDataAccessProvider(String location) {
        this.location = location;
    }

    public SimpleDataAccessProvider(File file) {
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

        // 创建本地数据源工厂
        JdbcDataSourceFactory dataSourceFactory = new JdbcDataSourceFactory();
        dataSourceFactory.setLocations(new Resource[] { configResource });

        return dataSourceFactory;
    }
}
