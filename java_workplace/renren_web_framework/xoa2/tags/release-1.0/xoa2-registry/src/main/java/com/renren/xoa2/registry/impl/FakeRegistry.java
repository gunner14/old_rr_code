package com.renren.xoa2.registry.impl;

import java.util.ArrayList;
import java.util.List;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.XoaRegistry;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 29, 2011 6:35:14 PM
 */
public class FakeRegistry implements XoaRegistry {

    @Override
    public List<Node> queryService(String serviceId, int shardParam) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public List<Node> queryService(String serviceId) {
        ArrayList<Node> nodes = new ArrayList<Node>();
        NodeImpl node = new NodeImpl();
        node.setHost("localhost");
        node.setPort(9090);
        nodes.add(node);
        return nodes;
    }
    
    @Override
    public List<String> queryAlarmPhones(String serviceId) {
       return null;
    }

}
