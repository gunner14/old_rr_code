package com.renren.xoa.server.coyote;

import java.util.HashMap;
import java.util.Iterator;
import java.util.concurrent.Executor;

import org.apache.coyote.Adapter;
import org.apache.coyote.ProtocolHandler;
import org.apache.coyote.Request;
import org.apache.coyote.Response;
import org.apache.log4j.Logger;

import com.renren.xoa.server.XoaServer;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-2-23 上午11:16:02
 */
public class NettyHttpProtocolHandler implements ProtocolHandler, ProtocolHandlerAdapter {

	private Logger logger = Logger.getLogger(NettyHttpProtocolHandler.class);
	
	private Adapter adapter;
	
	protected HashMap<String, Object> attributes = new HashMap<String, Object>();
	
	private int port;
    
	private XoaServer xoaServer;
	
	private Executor executor;
	
	public NettyHttpProtocolHandler() {
		
	}
	
	@Override
	public void destroy() throws Exception {
		xoaServer.stop();
	}
	
	@Override
	public Adapter getAdapter() {
		return adapter;
	}

	@Override
	public Object getAttribute(String key) {
		return attributes.get(key);
	}

	@Override
	public Iterator<String> getAttributeNames() {
		return attributes.keySet().iterator();
	}

	@Override
	public void init() throws Exception {
		// TODO Auto-generated method stub
	}

	@Override
	public void pause() throws Exception {
		// TODO Auto-generated method stub
	}

	@Override
	public void resume() throws Exception {
		// TODO Auto-generated method stub
	}

	@Override
	public void setAdapter(Adapter adapter) {
		this.adapter = adapter;
	}

	@Override
	public void setAttribute(String name, Object value) {
		//System.err.println("setAttribute " + name + " " + value);
		if (logger.isDebugEnabled()) {
			logger.debug("setAttribute " + name + "=" + value);
		}
		attributes.put(name, value);
	}

	public void setPort(int port) {
		if (logger.isDebugEnabled()) {
			logger.debug("set port=" + port);
		}
        this.port = port;
    }
	
	public void setExecutor(Executor executor) {
		//System.out.println("setExecutor:" + executor);
		this.executor = executor;
	}
	
	public void coyoteService(Request request, Response response) {
		try {
			adapter.service(request, response);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void start() throws Exception {
		if (xoaServer == null) {
			xoaServer = new XoaServer(this, executor, port);
		}
		xoaServer.start();
	}

}
