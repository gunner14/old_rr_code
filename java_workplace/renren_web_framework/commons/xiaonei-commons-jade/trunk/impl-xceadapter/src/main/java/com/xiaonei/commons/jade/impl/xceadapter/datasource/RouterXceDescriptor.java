package com.xiaonei.commons.jade.impl.xceadapter.datasource;

/**
 * 实现包含路由的散表配置项。
 * 
 * @author han.liao
 */
public class RouterXceDescriptor extends DirectXceDescriptor {

    protected XceRouter dbRouter, partitionRouter;

    /**
     * 创建包含路由的散表配置项。
     * 
     * @param dbName - 数据源名称
     */
    public RouterXceDescriptor(String dbName) {
        super(dbName);
    }

    @Override
    public XceRouter getDbRouter() {
        return dbRouter;
    }

    /**
     * 设置散库的路由。
     * 
     * @param dbRouter - 散库的路由
     */
    public void setDbRouter(XceRouter dbRouter) {
        this.dbRouter = dbRouter;
    }

    @Override
    public XceRouter getPartitionRouter() {
        return partitionRouter;
    }

    /**
     * 设置散表的路由。
     * 
     * @param dbRouter - 散表的路由
     */
    public void setPartitionRouter(XceRouter partitionRouter) {
        this.partitionRouter = partitionRouter;
    }
}
