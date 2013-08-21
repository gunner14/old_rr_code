package com.renren.xoa2.client;

import com.renren.xoa2.registry.Node;

/**
 * 用于封装负载均衡逻辑
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 29, 2011 6:08:50 PM
 */
public interface LoadBalancer {

    /**
     * 给定serviceId，返回一个负载均衡后的节点
     * 
     * @param serviceId
     * @return
     */
    public Node getNode(String serviceId, String version);
    
    public Node getNode(String serviceId, String version, int shardParam);

    /**
     * TODO: 添加新的接口，从而在存在网络拓扑的情况下可以进行自适应的适配
     * 
     * @author yuan.liu1@renren-inc.com
     */
    // public Node getNode(String serviceId, String clientIp);
}
