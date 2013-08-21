package com.renren.xoa2.registry;

import java.util.List;

/**
 * Shard layer
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @date Dec 15, 2011 1:26:06 AM
 * 
 */
public interface Shard extends Comparable<Shard>{
    
    public int getId();
    
    public List<Node> getNodes();
}
