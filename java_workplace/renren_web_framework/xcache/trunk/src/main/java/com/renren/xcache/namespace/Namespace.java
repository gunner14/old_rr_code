package com.renren.xcache.namespace;

/**
 * 命名空间，指一个业务。
 * 每个命名空间都有一个name作为唯一标识，且能够映射到一个CachePool
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:37:06
 */
public interface Namespace {

	/**
	 * @return 获取当前{@link Namespace}的name
	 */
	public String getName();
	
	/**
	 * @return 获取当前{@link Namespace}所映射到的CachePool的name
	 */
	public String getAssociatedPoolName();
}
