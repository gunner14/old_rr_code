package com.xiaonei.platform.core.opt.base.chain;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.chain.Context;

/**
 * 
 * Web API相关的的一个Context的子接口
 * 
 * @author Li Weibo
 * @since 2009-2-6
 */
public interface WebContext extends Context {
	
	/**
	 * @return 当前请求的request对象
	 */
	public HttpServletRequest getRequest();
	
	/**
	 * @return 当前请求的response对象
	 */
	public HttpServletResponse getResponse();

}
