package com.renren.xmq;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TTransportException;

import com.renren.thriftex.transport.XMQTransport;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        XMQTransport producerTrans = new XMQTransport("tcp://10.11.21.185:56304", XMQTransport.ProducerTrans);
        try {
            producerTrans.open();
        } catch (TTransportException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        TProtocol protocol = new TCompactProtocol(producerTrans);
        XmqDemo.Client rpcClient = new XmqDemo.Client(protocol);
        
        try {
            rpcClient.incr(2);
        } catch (TException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.out.println( "Hello World!" );
    }
}
