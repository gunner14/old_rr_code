package com.renren.xoa2.client;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.XoaRegistry;
import com.renren.xoa2.registry.XoaRegistryFactory;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-12-22 下午04:30:21
 */
public class RoundRobinLoadBalancer implements LoadBalancer{

    private Map<String, AtomicInteger> counterMap = new ConcurrentHashMap<String, AtomicInteger>();
    
    private XoaRegistry xoaRegistry = XoaRegistryFactory.getInstance().getDefaultRegistry();
    
    public Node getNode(String serviceId) {
        List<Node> nodes = xoaRegistry.queryService(serviceId);
        return selectNode(serviceId, nodes);
    }
    
    public Node getNode(String serviceId, int shardParam) {
        List<Node> nodes = xoaRegistry.queryService(serviceId, shardParam);
        return selectNode(serviceId, nodes);
    }
    
    public void setXoaRegistry(XoaRegistry xoaRegistry) {
        this.xoaRegistry = xoaRegistry;
    }

    private Node selectNode(String serviceId, List<Node> nodes) {
        AtomicInteger counter = counterMap.get(serviceId);
        if (counter == null) {
            synchronized (serviceId.intern()) {
                counter = counterMap.get(serviceId);
                if (counter == null) {
                    counter = new AtomicInteger();
                    counterMap.put(serviceId, counter);
                }
            }
        }
        
        int count = counter.incrementAndGet();
        if (count > Integer.MAX_VALUE - 100000) {   //防上溢，打出点提前量
            //归零
            counter.set(0);
        }
        Node node = null;
        int steps = 0;
        while (steps++ < nodes.size()) {
        	node = nodes.get(count++ % nodes.size());
        	if (!node.isDisabled()) {
        		return node;
          }
          counter.incrementAndGet();
        }

        return null;
    }

}
