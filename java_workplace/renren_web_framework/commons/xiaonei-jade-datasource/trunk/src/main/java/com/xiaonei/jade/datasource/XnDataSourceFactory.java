package com.xiaonei.jade.datasource;

import java.util.Map;

import javax.sql.DataSource;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.context.spring.SpringDataSourceFactory;
import net.paoding.rose.jade.dataaccess.DataSourceFactory;
import net.paoding.rose.jade.statement.StatementMetaData;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

/**
 * 
 * @author 王志亮 [qieqie.wang@gmail.com]
 * 
 */
public class XnDataSourceFactory implements DataSourceFactory, ApplicationContextAware {

    private SpringDataSourceFactory spring;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        spring = new SpringDataSourceFactory(applicationContext);
    }

    public SpringDataSourceFactory getSpringDataSourceFactory() {
        return spring;
    }

    @Override
    public DataSource getDataSource(StatementMetaData metaData,
            Map<String, Object> runtimeProperties) {
        // 如果spring容器配置了相应的dataSource，则使用之
        DataSource dataSource = spring.getDataSource(metaData, runtimeProperties);
        if (dataSource != null) {
            // 这是为了支持那些在applicationContext配置XnDataSource的DAO，这里进行clone使用其catalog
            if (dataSource instanceof XnDataSource) {
                XnDataSource prototype = (XnDataSource) dataSource;
                return new XnDataSource(prototype.getCatalog(), metaData, runtimeProperties);
            }
            return dataSource;
        }
        // 如果catalog非空，则默认使用校内数据源
        Class<?> daoClass = metaData.getDAOMetaData().getDAOClass();
        String catalog = daoClass.getAnnotation(DAO.class).catalog();
        if (catalog != null && catalog.length() > 0) {
            return new XnDataSource(catalog, metaData, runtimeProperties);
        }
        // 返回null给调用者表示无法给出相应的数据源
        return null;
    }

}
