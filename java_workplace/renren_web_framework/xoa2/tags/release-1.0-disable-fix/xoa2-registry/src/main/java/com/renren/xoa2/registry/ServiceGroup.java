package com.renren.xoa2.registry;


/**
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @date Dec 15, 2011 1:26:02 AM
 * 
 */
public interface ServiceGroup {

    public static final int DEFAULT_SHARD_FACTOR = 1;
    
    public static final int DEFAULT_PRIORITY = 0;
    
    public int getId();
    
    public int getShardFactor();
    
    public int getPriority();
    
    public Shard getShard(int shardValue);
    
}
