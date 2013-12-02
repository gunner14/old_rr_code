package com.renren.thriftex.server;

import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.thriftex.transport.XMQTransport;

/**
 * Single thread XMQ server, used for the task which need sequence processing.
 * 
 * @author yuan.liu1@renren-inc.com
 *
 */
public class XMQSimpleServer extends TServer {

    public static class XMQArgs extends AbstractServerArgs<XMQArgs> {
        String endpoint;
        
        public XMQArgs() {
            super(null);
        }
        
        public XMQArgs endpoint(String ep) {
            endpoint = ep;
            return this;
        }
    }
    
    private static final Logger LOGGER = 
            LoggerFactory.getLogger(XMQSimpleServer.class.getName());
    
    XMQTransport trans;
    
    public XMQSimpleServer(XMQArgs args) {
        super(args);
        trans = new XMQTransport(args.endpoint, XMQTransport.ConsumerTrans); 
    }

    protected boolean serveOne() {
        TTransport client = trans;
        TProcessor processor = null;
        TProtocol inputProtocol = null;
        TProtocol outputProtocol = null;

        try {
            if (client != null) {
                processor = processorFactory_.getProcessor(client);
                inputProtocol = inputProtocolFactory_.getProtocol(client);
                outputProtocol = outputProtocolFactory_.getProtocol(client);
                while (processor.process(inputProtocol, outputProtocol)) {}
            }
        } catch (TTransportException ttx) {
            LOGGER.error("Thrift transport error occurred during processing of message.", ttx);
            // Client died, just move on
        } catch (TException tx) {
            LOGGER.error("Thrift error occurred during processing of message.", tx);
        } catch (Exception x) {
            LOGGER.error("Error occurred during processing of message.", x);
        }
        
        return true;
    }
    
    @Override
    public void serve() {
        try {
            trans.open();
        } catch (TTransportException e) {
            LOGGER.error(e.getMessage());
        }

        setServing(true);
        
        LOGGER.info("%s is starting to serve", XMQSimpleServer.class.getName());
        
        while (serveOne()) {
     
        }
        
        setServing(false);
    }
}
