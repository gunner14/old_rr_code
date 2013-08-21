package com.renren.entrybase.zhan.datasource;


import java.util.Map;

import javax.sql.DataSource;

import net.paoding.rose.jade.dataaccess.DataSourceFactory;
import net.paoding.rose.jade.statement.StatementMetaData;

public class XnDataSourceFactory implements DataSourceFactory {

    private String catalog;

    public XnDataSourceFactory() {
    }

    public XnDataSourceFactory(String catalog) {
        this.catalog = catalog;
    }

    public void setCatalog(String catalog) {
        this.catalog = catalog;
    }

    @Override
    public DataSource getDataSource(StatementMetaData metaData,
            Map<String, Object> runtimeProperties) {
        return new XnDataSource(catalog, metaData, runtimeProperties);
    }

}

