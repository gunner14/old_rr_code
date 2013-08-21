package com.renren.xoa2.registry.impl.zookeeper;

import java.io.IOException;

import org.apache.zookeeper.ZooKeeper;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class XmqRegistryTest extends XmqRegistry {    
    private static ZooKeeper zookeeper = null;
    
    @Before
    public void setUp() throws Exception {
        try {
            zookeeper = new ZooKeeper(ZOOKEEPER_ADDR, 30000, null);
            Thread.sleep(500);

            PathUtil.createNode(zookeeper, "/xmq_unittest1", 
                    "".getBytes(), false);
            PathUtil.createNode(zookeeper, "/xmq_unittest1/Writer", 
                    "".getBytes(), false);
            PathUtil.createNode(zookeeper, "/xmq_unittest1/Writer/1.0.0.1:10000", 
                    "".getBytes(), true); 
            PathUtil.createNode(zookeeper, "/xmq_unittest1/Writer/1.0.0.1:10001", 
                    "".getBytes(), true); 
            

            PathUtil.createNode(zookeeper, "/xmq_unittest1/GateWay", 
                    "".getBytes(), false);
            PathUtil.createNode(zookeeper, "/xmq_unittest1/GateWay/1.0.0.1:20000", 
                    "".getBytes(), true);
            PathUtil.createNode(zookeeper, "/xmq_unittest1/GateWay/1.0.0.1:20001", 
                    "".getBytes(), true);
        } catch (IOException e) {
            throw new RuntimeException("Error occurs while creating ZooKeeper instance.", e);
        }
    }

    @After
    public void tearDown() throws Exception {
        zookeeper.close();
    }
    
    @Test
    public void Test() {
        XmqRegistry reg = new XmqRegistry();
        String ep = reg.getEndpoint("xmq_unittest1", XmqEndpointType.WRITER);
        assert(ep == "1.0.0.1:10000");

        ep = reg.getEndpoint("xmq_unittest1", XmqEndpointType.GATEWAY);        
        assert(ep == "1.0.0.1:20000");
        ep = reg.getEndpoint("xmq_unittest1", XmqEndpointType.GATEWAY);        
        assert(ep == "1.0.0.1:20000");
        ep = reg.getEndpoint("xmq_unittest1", XmqEndpointType.GATEWAY);        
        assert(ep == "1.0.0.1:20000");
        ep = reg.getEndpoint("xmq_unittest1", XmqEndpointType.GATEWAY);        
        assert(ep == "1.0.0.1:20000");
    }
}
