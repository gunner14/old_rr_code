package com.renren.xoa.registry.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import com.renren.xoa.registry.AbstractXoaRegistry;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.XoaServiceDescriptorBase;

/**
 * 简单的XoaRegistry实现，可用于本地调试
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-25 上午11:49:30
 */
public class SimpleXoaRegistry extends AbstractXoaRegistry {
	
    private Map<String, Set<XoaServiceDescriptor>> services = new HashMap<String, Set<XoaServiceDescriptor>>();
	
	/**
	 * 向registry注册一个服务节点
	 * 
	 * @param serviceId
	 * @param ipAddress
	 * @param port
	 */
	public void register(String serviceId, String ipAddress, int port) {
		register(serviceId, ipAddress, port, false);
	}
	
	public void register(String serviceId, String ipAddress, int port, boolean disabled) {
        
	    Set<XoaServiceDescriptor> nodes = services.get(serviceId);
        if (nodes == null) {
            nodes = new TreeSet<XoaServiceDescriptor>(XoaServiceDescriptor.COMPARATOR);
        }
	    XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
        desc.setServiceId(serviceId).setIpAddress(ipAddress).setPort(port).setDisabled(disabled);
        nodes.add(desc);
        services.put(serviceId, nodes);
    }

	@Override
	public List<XoaServiceDescriptor> getServiceNodes(String serviceId) {
		return new ArrayList<XoaServiceDescriptor>(services.get(serviceId));
	}

	public void disableNode(String serviceId, String ip, int port) {
	    Set<XoaServiceDescriptor> nodeSet = services.get(serviceId);
        XoaServiceDescriptor toDisable = null;
        for (XoaServiceDescriptor node : nodeSet) {
            if (node.getIpAddress().equals(ip) && node.getPort() == port) {
                toDisable = node;
            }
        }
        
        if (toDisable != null) {
            ((XoaServiceDescriptorBase)toDisable).setDisabled(true);
            updateServiceNodes(serviceId, new ArrayList<XoaServiceDescriptor>(nodeSet));
        }
	}
	
	public void deleteNode(String serviceId, String ip, int port) {
	    
	    Set<XoaServiceDescriptor> nodeSet = services.get(serviceId);
	    XoaServiceDescriptor toDelete = null;
	    for (XoaServiceDescriptor node : nodeSet) {
	        if (node.getIpAddress().equals(ip) && node.getPort() == port) {
	            toDelete = node;
	        }
	    }
	    if (toDelete != null) {
	        nodeSet.remove(toDelete);
	        updateServiceNodes(serviceId, new ArrayList<XoaServiceDescriptor>(nodeSet));
	    }
	}
	
	/**
     * 注册一个XoaServiceDescriptor
     * @param desc
     * @deprecated
     */
    public void register(XoaServiceDescriptor desc) {
        throw new UnsupportedOperationException("This method is deprecated. " +
                "Use register(String serviceId, String ipAddress, int port) instead.");
    }
}
