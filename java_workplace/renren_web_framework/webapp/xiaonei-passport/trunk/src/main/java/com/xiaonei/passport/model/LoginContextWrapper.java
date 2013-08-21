package com.xiaonei.passport.model;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginLocalVar;
/**
 * 由于客户端可能通过xoa调用,所以尽可能把环境参数变小.为了以后扩展,增加一些可选参数.一旦需要这些参数,则有调用方修改
 * @author Chris.Yuan
 *
 */
public class LoginContextWrapper implements ILoginContext{
	private Map<String, Object> cache=new HashMap<String, Object>();//可选
	private com.renren.passport.service.LoginContext context=new com.renren.passport.service.LoginContext();
	public LoginContextWrapper(com.renren.passport.service.LoginContext context){
		this.context=context;
	}
	public static ILoginLocalVar newLoginLocalVar(){
		return new LoginContextWrapper();
	}
	private LoginContextWrapper (){}
	@Override
	public Object getAttribute(String key) {
		return cache.get(key);
	}
	@Override
	public void setAttribute(String key, Object value) {
		cache.put(key, value);
	}
	@Override
	public void removeAttribute(String key) {
		cache.remove(key);
	}
	@Override
	public Map<String, String> getCooikes() {
		return context.getCooikes();
	}
	@Override
	public String getHost() {
		return context.getHost();
	}
	@Override
	public String getIp() {
		return context.getIp();
	}
	@Override
	public String getReferer() {
		return context.getReferer();
	}
	@Override
	public String getRequestUrl() {
		return context.getRequestUrl();
	}
	@Override
	public String getUserAgent() {
		return context.getUserAgent();
	}
}
