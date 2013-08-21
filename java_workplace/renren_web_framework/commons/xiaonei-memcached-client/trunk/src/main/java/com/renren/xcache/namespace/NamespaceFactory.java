package com.renren.xcache.namespace;

/**
 * 负责{@link Namespace}的创建
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:39:31
 */
public interface NamespaceFactory {

	/**
	 * 根据指定的name获取与之对应的{@link Namespace}实例
	 * @param name
	 * @return
	 */
	public Namespace getNamespace(String name);
	
}
