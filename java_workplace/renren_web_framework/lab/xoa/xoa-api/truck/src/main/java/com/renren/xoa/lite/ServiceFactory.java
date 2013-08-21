package com.renren.xoa.lite;

import com.renren.xoa.XoaClientI;

/**
 * 对Service接口进行封装的工厂
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-19 下午06:22:21
 */
public interface ServiceFactory {
	 
	public <T> T getService(Class<T> serviceInterface);
	
	public <T> T getService(Class<T> serviceInterface, XoaClientI client);
	
}
