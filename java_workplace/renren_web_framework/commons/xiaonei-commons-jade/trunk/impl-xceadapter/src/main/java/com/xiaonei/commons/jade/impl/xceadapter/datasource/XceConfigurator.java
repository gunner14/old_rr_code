package com.xiaonei.commons.jade.impl.xceadapter.datasource;

/**
 * 存放: XceDataSource 的配置信息。
 * 
 * @author han.liao
 */
public interface XceConfigurator {

    /**
     * 返回 Catalog 的配置信息。
     * 
     * @param catalog - 模块名称
     * 
     * @return 数据表的配置信息
     */
    XceDescriptor getCatalogDescriptor(String catalog);

    /**
     * 返回数据表的配置信息。
     * 
     * @param catalog - 模块名称
     * 
     * @param name - 数据表名称
     * 
     * @return 数据表的配置信息
     */
    XceDescriptor getDescriptor(String catalog, String name);
}
