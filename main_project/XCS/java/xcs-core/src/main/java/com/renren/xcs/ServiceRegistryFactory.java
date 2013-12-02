package com.renren.xcs;

/**
 * ServiceRegistry工厂
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public interface ServiceRegistryFactory {

    /**
     * 构建ServiceRegistry
     * 
     * @param config 配置
     * @param sessionTimeout ServiceRegistry session的链接超时时间
     * @return ServiceRegistry
     * @throws XcsException 创建失败
     */
    public ServiceRegistry getServiceRegistry(Configuration config);

    /**
     * 构建ServiceRegistryAccessor
     * 
     * @param config 配置
     * @param service 当前ServiceRegistryAccessor需要访问的service
     * @param version 当前ServiceRegistryAccessor需要访问的service的version
     * @param sessionTimeout session的链接超时时间
     * @return ServiceRegistryAccessor
     * @throws XcsException 创建失败
     */
    public ServiceRegistryAccessor getServiceRegistryAccessor(Configuration config, String service,
            String version);;
}
