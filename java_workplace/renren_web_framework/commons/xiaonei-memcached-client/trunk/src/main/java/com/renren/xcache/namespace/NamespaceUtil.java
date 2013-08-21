package com.renren.xcache.namespace;

import java.io.InputStream;
import java.util.Properties;

/**
 * 读取{@link Namespace}配置的工具
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-20 下午06:56:26
 */
public class NamespaceUtil {

	/**
	 * 从配置中加载一个{@link Namespace}实例
	 * 
	 * @param name
	 * @param configIn
	 * @return
	 * @throws Exception
	 */
	public static Namespace loadFromConfig(String name, InputStream configIn)
			throws Exception {
		NamespaceImpl namespace = new NamespaceImpl();
		Properties p = new Properties();
		p.load(configIn);
		String poolName = p.getProperty("pool");
		if (poolName == null || poolName.trim().length() == 0) {
			throw new IllegalArgumentException("Bad config for Namespace "
					+ name + ": pool=" + poolName);
		}
		namespace.setName(name);
		namespace.setAssociatedPoolName(poolName);
		return namespace;
	}
	
}
