package com.xiaonei.commons.jade.impl.xceadapter.datasource;

/**
 * 实现简单的散表配置项。
 * 
 * @author han.liao
 */
public class DirectXceDescriptor implements XceDescriptor {

    protected String dbName;

    /**
     * 创建简单的散表配置项。
     * 
     * @param dbName - 数据源名称
     */
    public DirectXceDescriptor(String dbName) {
        this.dbName = dbName;
    }

    @Override
    public String getDbName() {
        return dbName;
    }

    /**
     * 设置数据源名称。
     * 
     * @param dbName - 数据源名称
     */
    public void setDbName(String dbName) {
        this.dbName = dbName;
    }

    @Override
    public XceRouter getDbRouter() {
        return null; // 没有路由
    }

    @Override
    public XceRouter getPartitionRouter() {
        return null; // 没有路由
    }
}
