package com.renren.xcs;

/**
 * 基于Find实现的ServiceRegistryFactory
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public class FindServiceRegistryFactory implements ServiceRegistryFactory {

    @Override
    public ServiceRegistry getServiceRegistry(Configuration configuration) {
        return new FindServiceRegistry(configuration);
    }

    @Override
    public ServiceRegistryAccessor getServiceRegistryAccessor(Configuration configuration,
            String service, String version){
        return new FindServiceRegistryAccessor(service, version, configuration);
    }

}
