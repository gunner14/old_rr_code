package com.renren.xoa2.registry;

import com.renren.xoa2.registry.impl.FindXoaRegistry;


/**
 * 
 * XoaRegistry工厂
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-25 上午11:42:55
 */
public class XoaRegistryFactory {

	private static XoaRegistryFactory instance = new XoaRegistryFactory();
	
	public static XoaRegistryFactory getInstance() {
		return instance;
	}
	
	private XoaRegistry registry;
	
	private XoaRegistryFactory(){
		try {
			registry = new FindXoaRegistry();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * @return 默认的XoaRegistry
	 */
	public XoaRegistry getDefaultRegistry() {
		return registry;
	}
}
