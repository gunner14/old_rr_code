package com.renren.xoa2.registry.impl;

import java.util.List;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.Shard;

/**
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @date Dec 15, 2011 1:26:02 AM
 * 
 */
public class ShardImpl implements Shard {

    private int id;
    
    private List<Node> nodes;
    
    public void setId(int id) {
        this.id = id;
    }
    
    public void setNodes(List<Node> nodes) {
        this.nodes = nodes;
    }

    @Override
    public int getId() {
        return id;
    }

    @Override
    public List<Node> getNodes() {
        return nodes;
    }

    @Override
    public int compareTo(Shard o) {
        if (this.getId() < o.getId()) {
            return -1;
        } else if (this.getId() > o.getId()) {
            return 1;
        } else {
            return 0;
        }
    }

}
