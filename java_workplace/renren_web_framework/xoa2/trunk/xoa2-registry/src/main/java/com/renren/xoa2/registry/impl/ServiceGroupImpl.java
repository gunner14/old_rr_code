package com.renren.xoa2.registry.impl;

import java.util.List;

import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.Shard;

/**
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @date Dec 15, 2011 1:26:02 AM
 * 
 */
public class ServiceGroupImpl implements ServiceGroup {

    private int id;
    
    private int shardFactor = DEFAULT_SHARD_FACTOR;

    private int priority;
    
    private List<Shard> shards;
    
    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public int getShardFactor() {
        return shardFactor;
    }
    
    public void setShardFactor(int shardFactor) {
        this.shardFactor = shardFactor > 0 ? shardFactor : 1;
    }
    
    public List<Shard> getShards() {
        return shards;
    }
    
    public void setShards(List<Shard> shards) {
        this.shards = shards;
    }

    @Override
    public Shard getShard(int shardValue) {
        return shards.get(shardValue % this.shardFactor);
    }
    
    public int getPriority() {
        return priority;
    }
    
    public void setPriority(int priority) {
        this.priority = priority;
    }
    
}
