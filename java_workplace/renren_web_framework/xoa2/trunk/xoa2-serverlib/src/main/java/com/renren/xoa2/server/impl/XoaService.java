/**
 * 
 */

package com.renren.xoa2.server.impl;

import org.apache.thrift.TProcessor;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TServerTransport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * XoaService对象表示一个运行的xoa服务，由<code>IServiceBuilder</code>构建而成
 * 
 * @author jinchao.wen
 * @email jinchao.wen@opi-corp.com
 * @date 2011-12-7
 */
public class XoaService implements Runnable {

    private Logger logger = LoggerFactory.getLogger(XoaService.class);

    private TProcessor processor;

    private TServerTransport serverTransport;

    private TServer server;
    
    private XoaServiceConfig config;

    //construct by XoaServiceBuilder
    protected XoaService() {
    };

    @Override
    public void run() {
        try {
        	logger.info( config + " started.");
        	server.serve();
        	logger.info( config + " stoped.");
        } catch (Throwable t) {
        	logger.error("Exception occur: ", t);
        }
    }

    //============ getter and setter
    public TProcessor getProcessor() {
        return processor;
    }

    public TServerTransport getServerTransport() {
        return serverTransport;
    }

    public TServer getServer() {
        return server;
    }

    public void setProcessor(TProcessor processor) {
        this.processor = processor;
    }

    public void setServerTransport(TServerTransport serverTransport) {
        this.serverTransport = serverTransport;
    }

    public void setServer(TServer server) {
        this.server = server;
    }
    
    public XoaServiceConfig getConfig() {
        return config;
    }
    
    public void setConfig(XoaServiceConfig config) {
        this.config = config;
    }
}
