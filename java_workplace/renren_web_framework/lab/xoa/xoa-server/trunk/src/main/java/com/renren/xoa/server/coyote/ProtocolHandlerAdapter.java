package com.renren.xoa.server.coyote;

import org.apache.coyote.Request;
import org.apache.coyote.Response;

/**
 * ProtocolHandler适配器，这个适配器能将底层通讯框架提供的request和response
 * 封装后交给tomcat去处理
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-2 下午02:41:16
 */
public interface ProtocolHandlerAdapter {

	/**
	 * 将request和response交给coyote引擎去处理
	 * @param request
	 * @param response
	 */
	public void coyoteService(Request request, Response response);
	
}
