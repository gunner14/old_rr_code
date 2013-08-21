package com.xiaonei.commons.jade.impl.xceadapter.provider;

import javax.sql.DataSource;

import net.paoding.rose.jade.jadeinterface.datasource.DataSourceFactory;
import net.paoding.rose.jade.jadeinterface.provider.DataAccess;
import net.paoding.rose.jade.jadeinterface.provider.exql.ExqlDataAccessProvider;

import com.xiaonei.commons.jade.impl.xceadapter.configurator.RemoteXmlDocXceConfigurator;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceDataSourceFactory;

/**
 * 提供线上 xceadapter 版本的
 * {@link net.paoding.rose.jade.jadeinterface.provider.DataAccessProvider}
 * 实现。
 * 
 * @author han.liao
 */
public class XceDataAccessProvider extends ExqlDataAccessProvider {

    @Override
    protected DataAccess createDataAccess(DataSource dataSource) {

        DataAccess dataAccess = super.createDataAccess(dataSource);
        return new XceDataAccess(dataAccess);
    }

    @Override
    protected DataSourceFactory createDataSourceFactory() {

        // 创建数据源工厂
        XceDataSourceFactory dataSourceFactory = new XceDataSourceFactory();
        dataSourceFactory.setConfigurator(new RemoteXmlDocXceConfigurator());

        return dataSourceFactory;
    }
}
