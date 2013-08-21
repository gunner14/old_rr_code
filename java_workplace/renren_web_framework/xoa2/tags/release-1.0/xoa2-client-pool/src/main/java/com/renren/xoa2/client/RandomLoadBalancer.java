package com.renren.xoa2.client;

import java.util.List;
import java.util.Random;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.XoaRegistry;
import com.renren.xoa2.registry.XoaRegistryFactory;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 29, 2011 6:12:23 PM
 *  
 * @deprecated 用于测试 
 */
public class RandomLoadBalancer implements LoadBalancer {

    private XoaRegistry registry = XoaRegistryFactory.getInstance().getDefaultRegistry();

    @Override
    public Node getNode(String serviceId) {
        List<Node> nodes = registry.queryService(serviceId);
        // 这里实现的是随机的负载均衡策略
        Random rand = new Random();
        return nodes.get(rand.nextInt(nodes.size()));
    }

    @Override
    public Node getNode(String serviceId, int shardParam) {
        List<Node> nodes = registry.queryService(serviceId, shardParam);
        // 这里实现的是随机的负载均衡策略
        Random rand = new Random();
        return nodes.get(rand.nextInt(nodes.size()));
    }

}
