package com.renren.xmq;

import java.nio.ByteBuffer;

import org.apache.thrift.TProcessor;
// import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TCompactProtocol;

import com.renren.thriftex.server.XMQSimpleServer;
import com.renren.thriftex.server.XMQSimpleServer.XMQArgs;
import com.renren.xoa2.registry.impl.zookeeper.XmqEndpointType;
import com.renren.xoa2.registry.impl.zookeeper.XmqRegistry;

public class XmqDemoImpl implements IXmqDemo {

    public void incr(int amount) {
        System.out.println("incr " + amount);        
    }
    
    public void sendData(ByteBuffer data) {
        System.out.println("data length = " + data.array().length);
    }
    
    public int get() {
        return 0;
    }

    public static void main(String[] args) {
        XMQArgs mqargs = new XMQArgs();
        
        TProcessor processor = new XmqDemo.Processor(new XmqDemoImpl());
        mqargs.processor(processor);
        mqargs.protocolFactory(new TCompactProtocol.Factory());
        
        XmqRegistry reg = new XmqRegistry();
        mqargs.endpoint("tcp://" + reg.getEndpoint("xmqDemo", XmqEndpointType.WRITER));        
        
        XMQSimpleServer server = new XMQSimpleServer(mqargs);       

        server.serve();
    }
}
