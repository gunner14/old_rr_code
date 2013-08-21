/**
 * 
 */

package com.renren.xoa2.server.impl;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.thrift.TProcessor;
import org.apache.thrift.server.THsHaServer;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TNonblockingServerSocket;
import org.apache.thrift.transport.TNonblockingServerTransport;
import org.apache.thrift.transport.TServerTransport;
import org.apache.thrift.transport.TTransportException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.server.IInitListener;
import com.renren.xoa2.server.processor.XoaProcessor;

/**
 * @author jinchao.wen
 * @email jinchao.wen@opi-corp.com
 * @date 2011-12-7
 */
public class ServiceBuilderImpl extends AbstractServiceBuilder {
    
    private static Logger logger = LoggerFactory.getLogger(ServiceBuilderImpl.class);

    /* (non-Javadoc)
     * @see com.renren.xoa2.server.impl.AbstractServiceBuilder#buildProcessor(com.renren.xoa2.server.impl.XoaServiceConfig)
     */
    @Override
    protected TProcessor buildProcessor(XoaServiceConfig config) {

        final String className = config.getServiceClass();

        if (!className.endsWith("Impl")) {
            throw new IllegalArgumentException("Service class name must be end with 'Impl': "
                    + className);
        }
        String interfaceClassName = className.substring(0, className.length() - "Impl".length());

        try {
            Class<?> ifaceClass = Class.forName(interfaceClassName + "$Iface");
            Class<?> processorClass = Class.forName(interfaceClassName + "$Processor");

            Constructor<?> constructor = processorClass.getConstructor(ifaceClass);
            Object impl = Class.forName(className).newInstance();
            TProcessor processor = (TProcessor) constructor.newInstance(impl);

            return new XoaProcessor(processor);

        } catch (ClassNotFoundException e) {
            logger.error("", e);
        } catch (SecurityException e) {
            logger.error("", e);
        } catch (NoSuchMethodException e) {
            logger.error("", e);
        } catch (InstantiationException e) {
            logger.error("", e);
        } catch (IllegalAccessException e) {
            logger.error("", e);
        } catch (IllegalArgumentException e) {
            logger.error("", e);
        } catch (InvocationTargetException e) {
            logger.error("", e);
        }
        return null;
    }

    /* (non-Javadoc)
     * @see com.renren.xoa2.server.impl.AbstractServiceBuilder#buildTServerTransport(com.renren.xoa2.server.impl.XoaServiceConfig)
     */
    @Override
    protected TServerTransport buildTServerTransport(XoaServiceConfig config) {

        final int port = config.getPort();

        try {
            TServerTransport serverTransport = new TNonblockingServerSocket(port);
            return serverTransport;
        } catch (TTransportException e) {
            logger.error("", e);
        }

        return null;
    }
    
	ExecutorService createExecuteService(XoaServiceConfig config) {
		int coreSize = config.getThreadPoolCoreSize(); 
		int maxSize = config.getThreadPoolMaxSize();
		
		if (coreSize <= 0 || maxSize <= 0) {
			coreSize = 2 * Runtime.getRuntime().availableProcessors();
			maxSize = 2 * coreSize;
		}

		LinkedBlockingQueue<Runnable> queue = new LinkedBlockingQueue<Runnable>();
		
		XoaThreadFactory threadFactory = new XoaThreadFactory();
		
		ExecutorService invoker = new ThreadPoolExecutor(coreSize, maxSize, 60,
				TimeUnit.SECONDS, queue, threadFactory);

		return invoker;
	}
    

    /* (non-Javadoc)
     * @see com.renren.xoa2.server.impl.AbstractServiceBuilder#buildTServer(com.renren.xoa2.server.impl.XoaServiceConfig)
     */
    @Override
    protected TServer buildTServer(XoaServiceConfig config, TServerTransport serverTransport,
            TProcessor processor) {

        THsHaServer.Args args = new THsHaServer.Args((TNonblockingServerTransport) serverTransport);
        args.executorService(createExecuteService(config));        
        args.processor(processor);

        return new THsHaServer( args );
    }

    /* (non-Javadoc)
     * @see com.renren.xoa2.server.impl.AbstractServiceBuilder#buildInitListener(com.renren.xoa2.server.impl.XoaServiceConfig)
     */
    @Override
    protected IInitListener buildInitListener(XoaServiceConfig config) {

        final String listenerClassName = config.getInitListenerClass();

        if (null != listenerClassName && !listenerClassName.trim().isEmpty()) {

            try {
                Class<?> clazz = Class.forName(listenerClassName);
                Object listenerObj = clazz.newInstance();
                if (listenerObj instanceof IInitListener) {
                    return (IInitListener) listenerObj;
                }
            } catch (ClassNotFoundException e) {
                logger.error("", e);
            } catch (InstantiationException e) {
                logger.error("", e);
            } catch (IllegalAccessException e) {
                logger.error("", e);
            }
        }

        return null;
    }
}
