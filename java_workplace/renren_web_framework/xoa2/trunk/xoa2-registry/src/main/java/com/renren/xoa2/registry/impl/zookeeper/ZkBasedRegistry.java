package com.renren.xoa2.registry.impl.zookeeper;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.Service;
import com.renren.xoa2.registry.impl.AbstractXoaRegistry;

public class ZkBasedRegistry extends AbstractXoaRegistry {	
    @Override
    protected Service loadService(String serviceId) {
        String nodePath = PathUtil.serviceIdToPath(serviceId);

        ServiceWatcher service = new ServiceWatcher(nodePath);
        if (service.load()) {
        	return service;
        }
        return null;
    }
    
    @Override
    public boolean registerService(String serviceId, int serviceGroupId, int shardParam, Node node) {
        String nodePath = PathUtil.serviceIdToPath(serviceId);

        ServiceWatcher service = new ServiceWatcher(nodePath);
        return service.registerNode(serviceGroupId, shardParam, node);
    }

    @Override
    public boolean disableService(String serviceId, int serviceGroupId, int shardParam, Node node) {
    	String nodePath = PathUtil.serviceIdToPath(serviceId);

        ServiceWatcher service = new ServiceWatcher(nodePath);
        return service.disableNode(serviceGroupId, shardParam, node);
    }
}
