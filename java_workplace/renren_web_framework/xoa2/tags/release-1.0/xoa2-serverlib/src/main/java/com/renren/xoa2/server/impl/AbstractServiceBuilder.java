/**
 * 
 */

package com.renren.xoa2.server.impl;

import org.apache.thrift.TProcessor;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TServerTransport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.server.IInitListener;
import com.renren.xoa2.server.IServiceBuilder;

/**
 * @author jinchao.wen
 * @email jinchao.wen@opi-corp.com
 * @date 2011-12-7
 */
public abstract class AbstractServiceBuilder implements IServiceBuilder {
    
    private static Logger logger = LoggerFactory.getLogger(AbstractServiceBuilder.class);

    /**
     * 由config生成TProcessor
     * @param config
     * @return
     */
    protected abstract TProcessor buildProcessor(XoaServiceConfig config,XoaService xoaService);

    /**
     * 由config生成TServerTransport，使用TNonblockingServerSocket
     * @param config
     * @return
     */
    protected abstract TServerTransport buildTServerTransport(XoaServiceConfig config);

    /**
     * 由config对象生成TServer，使用THsHaServer
     * @param config
     * @param serverTransport
     * @param processor
     * @return
     */
    protected abstract TServer buildTServer(XoaServiceConfig config,
            TServerTransport serverTransport, TProcessor processor);

    /**
     * 由config对象生成IInitListener
     * @param config
     * @return
     */
    protected abstract IInitListener buildInitListener(XoaServiceConfig config);
    
    protected abstract void setAbstractServer(TServer server, XoaService xoaService);
    

    @Override
    public final XoaService build(XoaServiceConfig config, boolean isMilitia) {
        if( logger.isDebugEnabled() ){
            logger.debug("start build service: " + config);
        }
        XoaService xoaService = new XoaService();
        xoaService.setConfig(config);
        
        TProcessor processor = this.buildProcessor(config, xoaService);
        if( processor != null){
            xoaService.setProcessor(processor);
        }else{
            logger.error(" builProcessor ERROR, sysytem exit with 1. please check config: " + config);
            //Fail fast
            if(!isMilitia) {
                System.exit(1);
            } else {
                return null;
            }
        }
        
        TServerTransport serverTransport = null;
        if (!config.isXmqService()) {
            serverTransport = this.buildTServerTransport(config);
            if (serverTransport != null) {
                xoaService.setServerTransport(serverTransport);
            } else {
                logger.error(" buildTServerTransport ERROR, sysytem exit with 1. please check config: "
                        + config);
                // Fail fast
                if(!isMilitia) {
                    System.exit(1);
                } else {
                    return null;
                }
            }
        }
        

        TServer server = this.buildTServer(config, serverTransport, processor);
        
        
        if (server != null) {  
            setAbstractServer(server,xoaService);    
            xoaService.setServer(server);
        } else {
            logger.error(" buildTServer ERROR, sysytem exit with 1. please check config: "
                    + config);
            // Fail fast
            if(!isMilitia) {
                System.exit(1);
            } else {
                return null;
            }
        }
       
        
        IInitListener initListener = this.buildInitListener(config);
        if (null != initListener) {
            initListener.update(xoaService, null);
        }
       
        return xoaService;
    }

}
