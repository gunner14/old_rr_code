package com.renren.xcache.pool;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * {@link CachePool}相关工具
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:13:36
 */
public class CachePoolUtil {

	/**
	 * 从配置中加载{@link CachePool}
	 * 
	 * @param poolName
	 * @param configIn
	 * @return
	 * @throws Exception
	 */
	public static CachePool loadFromConfig(String poolName, InputStream configIn) throws Exception{
	
		BufferedReader reader = new BufferedReader(new InputStreamReader(configIn));
		String line;
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName(poolName);
		while ((line = reader.readLine()) != null) {
			line = line.trim();
			if (line.length() > 0 && !line.startsWith("#")) {
				//格式是hostname:port*weight
				String[] ss1 = line.split(":");
				String hostname = ss1[0];
				String[] ss2 = ss1[1].split("\\*");
				int port = Integer.parseInt(ss2[0]);
				int weight = Integer.parseInt(ss2[1]);
				pool.addNode(new NodeImpl().setHostname(hostname).setPort(port).setWeight(weight));
			}
		}
		return pool;
	}
	
}
