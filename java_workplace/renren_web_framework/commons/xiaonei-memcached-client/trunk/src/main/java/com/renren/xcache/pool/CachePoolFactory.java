package com.renren.xcache.pool;


/**
 * 负责{@link CachePool}的创建
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:14:14
 */
public interface CachePoolFactory {

	/**
	 * 根据指定的poolName获取一个{@link CachePool}实例
	 * 
	 * @param poolName
	 * @return
	 */
	public CachePool getPool(String poolName);
	
	/**
	 * 向当前Factory中添加一个CachePool配置变更时的回调函数
	 * 
	 * @param callback
	 */
	public void addCallback(PoolConfigModifiedCallback callback);
	
	/**
	 * CachePool配置变更时的回调函数
	 */
	public interface PoolConfigModifiedCallback {
		public void onPoolModified(CachePool newPool);
	}
}
