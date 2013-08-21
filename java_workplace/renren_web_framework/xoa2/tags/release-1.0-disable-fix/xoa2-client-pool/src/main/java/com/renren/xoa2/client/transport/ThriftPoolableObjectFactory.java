/**
 * @ ThriftPool.java Create on 2011-9-15 上午11:07:29
 */
package com.renren.xoa2.client.transport;

import org.apache.commons.pool.PoolableObjectFactory;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @author gang.pan
 * @mail gang.pan@renren-inc.com
 * @version 1.0
 */

/*
 * @(#)ThriftPoolableObjectFactory.java 0.1 05/11/17 Copyright 2010 QISI, Inc.
 * All rights reserved. QISI PROPRIETARY/CONFIDENTIAL. Use is subject to license
 * terms.
 */
class ThriftPoolableObjectFactory implements PoolableObjectFactory {
    private Logger logger = LoggerFactory
                    .getLogger(ThriftPoolableObjectFactory.class);
    /** 服务的IP */
    private String serviceIP;

    /** 服务的端口 */
    private int servicePort;

    /** 超时设置 */
    private int timeout;

    /**
     * @param serviceIP
     * @param servicePort
     * @param timeOut
     */
    public ThriftPoolableObjectFactory(String serviceIP, int servicePort,
            int timeout) {
        this.serviceIP = serviceIP;
        this.servicePort = servicePort;
        this.timeout = timeout;
    }

    @Override
    public void destroyObject(Object arg0) throws Exception {
        if (arg0 instanceof TTransport) {
            TTransport transport = (TTransport) arg0;
            if (transport.isOpen()) {
                transport.close();
            }
        }
    }

    @Override
    public Object makeObject() throws Exception {
        try {
            TSocket socket = new TSocket(this.serviceIP, this.servicePort);
            socket.getSocket().setKeepAlive(true);
            socket.getSocket().setTcpNoDelay(true);
            socket.getSocket().setSoLinger(false, 0);
            socket.getSocket().setSoTimeout(this.timeout);
            TTransport transport = new TFramedTransport(socket);
            transport.open();
            if (logger.isDebugEnabled()) {
                logger.debug("client pool make object success.");
            }
            return transport;
        } catch (Exception e) {
            logger.warn("client pool make object error.");
            throw new RuntimeException(e);
        }
    }

    @Override
    public boolean validateObject(Object arg0) {
        try {
            if (arg0 instanceof TTransport) {
                TTransport transport = (TTransport) arg0;

                if (transport.isOpen()) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    @Override
    public void passivateObject(Object arg0) throws Exception {
        // DO NOTHING
    }

    @Override
    public void activateObject(Object arg0) throws Exception {
        // DO NOTHING
    }

    public String getServiceIP() {
        return serviceIP;
    }

    public void setServiceIP(String serviceIP) {
        this.serviceIP = serviceIP;
    }

    public int getServicePort() {
        return servicePort;
    }

    public void setServicePort(int servicePort) {
        this.servicePort = servicePort;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setTimeout(int timeout) {
        this.timeout = timeout;
    }
}
