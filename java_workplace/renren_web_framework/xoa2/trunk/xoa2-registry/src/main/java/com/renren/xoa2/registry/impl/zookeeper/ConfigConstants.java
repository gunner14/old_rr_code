package com.renren.xoa2.registry.impl.zookeeper;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-12-15 下午08:29:03
 */
public interface ConfigConstants {

    public static final String SHARD_PREFIX = "shard_";
    
    public static final String SERVICE_GROUP_PREFIX = ".service_group_";
    
    public static final String PROPERTY_KEY_SHARD_FACTOR = "shard_factor";
    
    public static final String PROPERTY_KEY_PRIORITY = "priority";
    
    public static final String PROPERTY_NODE_STATUS = "status";
    
    public static final String NODE_STATUS_ENABLED = "enabled";
}
