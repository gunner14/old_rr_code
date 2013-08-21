package com.renren.xoa2.registry.impl;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.Service;
import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.Shard;
import com.renren.xoa2.registry.XoaRegistry;

/**
 * XoaRegistry的一个基础实现类
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public abstract class AbstractXoaRegistry implements XoaRegistry {

    protected Log logger = LogFactory.getLog(this.getClass());

    private Map<String, Service> serviceMap = new ConcurrentHashMap<String, Service>();
    
    @Override
    public List<Node> queryService(String serviceId) {
        return queryService(serviceId, 1);
    }

    @Override
    public List<Node> queryService(String serviceId, int shardParam) {

        Service service = serviceMap.get(serviceId);
        if (service == null) {  //DCL
            synchronized (serviceId.intern()) {
                service = serviceMap.get(serviceId);
                if (service == null) {
                    
                    //load
                    service = loadService(serviceId);
                    
                    //put into map
                    if (service != null) {
                        serviceMap.put(serviceId, service);
                    }
                }
            }
        }
        
        //not found
        if (service == null) {
            return null;
        }
        ServiceGroup group = service.getServiceGroup();
        Shard shard = group.getShard(shardParam);
        return shard.getNodes();
    }
    
    protected abstract Service loadService(String serviceId);

    @Override
    public boolean registerService(String serviceId, Node node) {
    	return registerService(serviceId, 0, 0, node);
    }    

    @Override
    public boolean disableService(String serviceId, Node node) {
    	return disableService(serviceId, 0, 0, node);    	
    }
}
