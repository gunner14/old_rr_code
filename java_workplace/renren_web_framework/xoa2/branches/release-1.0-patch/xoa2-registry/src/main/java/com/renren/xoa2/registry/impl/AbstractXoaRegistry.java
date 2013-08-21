package com.renren.xoa2.registry.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.Service;
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
    public List<Node> queryService(String serviceId, String version) {
        return queryService(serviceId, version, 0);
    }
    
    private Service getService(String serviceId, String version, int shardParam) {
    	String serviceUid = serviceId + version + shardParam;
    	Service service = serviceMap.get(serviceUid);
        if (service == null) {  //DCL
            synchronized (serviceId.intern()) {
                service = serviceMap.get(serviceUid);
                if (service == null) {
                    
                    //load
                    service = loadService(serviceId, version, shardParam);
                    
                    //put into map
                    if (service != null) {
                        serviceMap.put(serviceUid, service);
                    }
                }
            }
        }
        
        return service;
    }
    
    @Override
    public List<String> queryAlarmPhones(String serviceId) {
        Service service = serviceMap.get(serviceId);
        if (service == null) {
        	return null;
        }
        
        return service.getAlarmPhones();
    }
    
    @Override
    public List<Node> queryService(String serviceId, String version, int shardParam) {
    	List<Node> propNodes = getHostConfigFromSysProp(serviceId, shardParam); 
    	if (!propNodes.isEmpty()) {
    		return propNodes;
    	}
        Service service = getService(serviceId, version, shardParam);        
        
        //not found
        if (service == null) {
            return null;
        }
        return service.getServiceList();
    }
    
    protected abstract Service loadService(String serviceId, String version, int shardParam);

    class LocalConfig {
    	public LocalConfig(List<Node> nodes, long time) {
    		this.nodes = nodes;
    		this.time = time;
    	}
    	public List<Node> nodes;
    	public long time;
    }
    private Map<String, LocalConfig> hostConfigCache = new HashMap<String, LocalConfig>();
    
    private List<Node> getLocalConfigCache(String serviceId) {
    	// TODO : thread safety
    	LocalConfig cfg = hostConfigCache.get(serviceId);
		long curTime = (new java.util.Date()).getTime();
		if (cfg != null &&  curTime - cfg.time < 10000) {
			return cfg.nodes;
		}
    	return null;
    }

	private List<Node> getHostConfigFromSysProp(String serviceId, int shard) {
		List<Node> nodes = getLocalConfigCache(serviceId);
		
		if (nodes != null) {
			return nodes;
		}
		
		StringBuilder propName = new StringBuilder();
		propName.append("xoa2.hosts.");
		if (shard > 0) {	// 支持shard策略
			propName.append(shard);
			propName.append('.');
		}
		propName.append(serviceId);
		
		String hosts = System.getProperty(propName.toString());
		
		nodes = new ArrayList<Node>();
		if (hosts != null) {
			String hostVec[] = hosts.split(",");
			for (String host : hostVec) {
				String[] ss = host.split(":");
				if (ss.length == 2) {					
					NodeImpl node = new NodeImpl();
					node.setDisabled(false);
					node.setHost(ss[0]);
					node.setPort(Integer.parseInt(ss[1]));
					nodes.add(node);
				}
			}
			if (nodes.size() > 0) {
				logger.warn("Using system property to locate XOA service nodes:" + hosts);
			}
			hostConfigCache.put(serviceId, new LocalConfig(nodes, (new java.util.Date()).getTime()));
		}
		return nodes;
	}

}
