package com.renren.xcache.pool;

import java.util.List;

/**
 * 用来抽象“缓存池”的概念，一个缓存池以name为唯一标识，并且包含一组memcached node。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:09:21
 */
public interface CachePool {

	/**
	 * @return 当前{@link CachePool}的名字
	 */
	public String getName();
	
	/**
	 * @return 当前{@link CachePool}中的所有节点
	 */
	public List<Node> getNodes();
	
}
