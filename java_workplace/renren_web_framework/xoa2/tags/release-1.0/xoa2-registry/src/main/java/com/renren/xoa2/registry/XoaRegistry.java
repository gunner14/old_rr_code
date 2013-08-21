package com.renren.xoa2.registry;

import java.util.List;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 28, 2011 4:41:12 PM
 */
public interface XoaRegistry {

    public List<Node> queryService(String serviceId);
    
    /**
     * 返回指定服务的服务器节点信息
     * 
     * @param serviceId
     * @param shardParam 用于做sharding的参数
     * @return 对应的节点列表
     */
    public List<Node> queryService(String serviceId, int shardParam);

    public List<String> queryAlarmPhones(String serviceId);    
}
