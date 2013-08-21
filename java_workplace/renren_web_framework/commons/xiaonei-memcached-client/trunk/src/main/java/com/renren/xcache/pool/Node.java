package com.renren.xcache.pool;

/**
 * 用来抽象{@link CachePool}中的memcached节点
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:16:33
 */
public interface Node {

	/**
	 * @return 节点机器的hostname
	 */
	public String getHostname();

	/**
	 * @return 节点端口
	 */
	public int getPort();

	/**
	 * @return 节点权重
	 */
	public int getWeight();

}
