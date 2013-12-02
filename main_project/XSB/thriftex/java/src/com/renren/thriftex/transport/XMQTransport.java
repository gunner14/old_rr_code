package com.renren.thriftex.transport;

import org.apache.thrift.TByteArrayOutputStream;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.zeromq.ZMQ;

import com.renren.thriftex.xmq.Constant;
import com.renren.thriftex.xmq.Message;

public class XMQTransport extends TTransport {
    public static final int ProducerTrans = 0;
    public static final int ConsumerTrans = 1;

    // default timeout (ns)
    public static final int XMQ_TimeOut = 500000;
    
    private String endPoint;
    private int connType;

    // local id, to identify the data package
    private long messageID;
    // producer UUID, generate by gateway
    byte[] producerUUID;

    ZMQ.Context context;
    ZMQ.Poller poller;
    ZMQ.Socket sock;
    
    boolean isTransportOpen = false;
    
    TByteArrayOutputStream byteArry;
    int pos = 0;
    
    final Logger logger = LoggerFactory.getLogger(XMQTransport.class.getName());
    
    public XMQTransport(String endpoint, int connectionType) {
        endPoint = endpoint;
        connType = connectionType;
        messageID = -1;
        producerUUID = new byte[0];
        byteArry = new TByteArrayOutputStream();
    }
    
    @Override
    public boolean isOpen() {
        return isTransportOpen;
    }

    @Override
    public void open() throws TTransportException {
        context = ZMQ.context(1);
        poller = context.poller(1);
        sock = context.socket(ZMQ.REQ);
        
        poller.register(sock, ZMQ.Poller.POLLIN);
        
        sock.connect(endPoint);
        if (connType == ProducerTrans) {
            if (openGateWay(messageID)) {
                messageID ++;
            } else {
                // error here
            }
        } else if (connType == ConsumerTrans) {
            openWriter();
        }
    }

    /**
     * Open the gateway connection and do login logic
     * 
     * @param seqID current
     */
    protected boolean openGateWay(long seqID) {
        sock.send(Message.packMessage(Constant.PRODUCER_LOGIN_CODE,
                seqID, producerUUID, producerUUID.length), 0);
        poller.poll(XMQ_TimeOut);
        if (poller.pollin(0)) {
            Message message = Message.unpackMessage(sock.recv(0));
            if (message.opCode == Constant.PRODUCER_LOGIN_ACK) {
                producerUUID = message.data;
                isTransportOpen = true;

                logger.info("connected to endpoint [%s], current uuid is [%s]",
                        endPoint, producerUUID);
                
                return true; 
            } else {
                logger.error("connected to GateWay endpoint [%s] failed!",
                             endPoint);
            }
        } 
        
        return false;
    }
    
    /** 
     * Open Writer and get the info
     */
    protected boolean openWriter() {
        sock.send(Message.packMessage(Constant.CONSUMER_LOGIN_CODE,
                0, producerUUID, producerUUID.length), 0);

        poller.poll(XMQ_TimeOut);
        if (poller.pollin(0)) {
            Message message = Message.unpackMessage(sock.recv(0));

            if (message.opCode == Constant.CONSUMER_LOGIN_ACK) {
                //TODO: process the data
                messageID ++;
                logger.info("connected to endpoint [%s], current range is TODO",
                        endPoint);
            } else {
                logger.error("connected to Writer endpoint [%s] failed!",
                        endPoint);
            }
        }
        
        return false;
    }
    
    @Override
    public void close() {
        if (connType == ProducerTrans) {
            // clean the data            
        }
        
        poller.unregister(sock);
        sock.close();
        context.term();
    }

    @Override
    public int read(byte[] buf, int off, int len) throws TTransportException {
        byte[] src = byteArry.get();
        int amtToRead = (len > byteArry.len() - pos ? byteArry.len() - pos : len);
        
        if (amtToRead > 0) {
            System.arraycopy(src, pos, buf, off, amtToRead);
            pos += amtToRead;
        } else {
            if (readMessage()) {
                return read(buf, off, len);
            } else {
                // the inner 
            }
        }
        
        return amtToRead;
    }
    
    /**
     * Block read message from writer
     * 
     * TODO: extent this to support multi-strategy
     * 
     * @return true : 
     *         false: 
     */
    protected boolean readMessage() throws TTransportException {
        pos = 0;
        byteArry = new TByteArrayOutputStream();
        
        while (true) {
            sock.send(Message.packMessage(Constant.CONSUMER_GET_DATA, 
                    messageID, producerUUID, producerUUID.length), 0);
            
            poller.poll(XMQ_TimeOut);
            if (poller.pollin(0)) {
                Message message = Message.unpackMessage(sock.recv(0));
                
                if (message.opCode == Constant.CONSUMER_DATA) {
                    byteArry.write(message.data, 0, message.data.length);
                    messageID ++;
                    break;
                } else if (message.opCode == Constant.CONSUMER_DATA_NOT_EXIST) {
                    logger.debug("message [%d] not exist yet, sleep 1 sec...",
                            messageID);

                    // sleep 1 second
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                } else {
                    return false;
                }
            } else {
                // TODO: connection may break, re-check 
            }
        }
        
        return true;
    }

    @Override
    public void write(byte[] buf, int off, int len) throws TTransportException {
        byteArry.write(buf, off, len);
        pos += len;
    }
    
    public void flush() {
        byte[] zmqMessage = Message.packMessage(Constant.PRODUCER_SEND_DATA, 
                messageID, byteArry.get(), pos);
        
        sock.send(zmqMessage, 0);

        pos = 0;
        byteArry = new TByteArrayOutputStream();

        // logging here
        poller.poll(XMQ_TimeOut);
        if (poller.pollin(0)) {
            Message message = Message.unpackMessage(sock.recv(0));
            if (message.opCode == Constant.GATEWAY_DATA_ACK) {
                // the data is safely send
                messageID ++;
            }
        } else {
            // timeout, need reconnect
            
        }
    }
    
    public static void main(String[] args) {
        XMQTransport trans = new XMQTransport("tcp://192.168.0.3:60627", 
                ProducerTrans);
        try {
            trans.open();
            trans.close();
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}