package com.renren.xcache.zookeeper;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;

import org.apache.zookeeper.ZooKeeper;

import com.renren.xcache.pool.CachePool;
import com.renren.xcache.pool.CachePoolUtil;
import com.renren.xcache.pool.ZooKeeperBasedPoolFactory;

/**
 * 向ZooKeeper中同步{@link CachePool}配置的工具
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-20 下午06:55:19
 */
public class PoolConfigMgr {

	
	public static void main(String[] args) throws Exception {
		
		if (args.length != 1) {
			System.err.println("Usage:java PoolConfigMgr local_config_file_path");
			return;
		}
		String configFilePath = args[0];
		File file = new File(configFilePath);
		String fileName = file.getName();
		if (!fileName.endsWith(ZooKeeperBasedPoolFactory.EXT)) {
			throw new IllegalArgumentException("File name must be *."
					+ ZooKeeperBasedPoolFactory.EXT + ", but yours:" + fileName);
		}
		String poolName = fileName.substring(0, fileName.length()
				- ZooKeeperBasedPoolFactory.EXT.length());
		
		FileInputStream fin = new FileInputStream(file);
		ByteArrayOutputStream bo = new ByteArrayOutputStream();
		byte[] buff = new byte[256];
		
		int bytesRead;
		while ((bytesRead = fin.read(buff)) > 0) {
			bo.write(buff, 0, bytesRead);
		}
		byte[] data = bo.toByteArray();
		
		System.out.println("config for pool '" + poolName + "':");
		System.out.println(new String(data, "UTF-8"));
		CachePool pool = CachePoolUtil.loadFromConfig(poolName, new ByteArrayInputStream(data));
		System.out.println(pool.toString());
		
		ZooKeeper zookeeper = ZooKeeperFactory.newZooKeeper(null);
		
		String path = ZooKeeperBasedPoolFactory.ROOT_PATH + poolName + ZooKeeperBasedPoolFactory.EXT;
		System.out.println("set data to:" + path);
		boolean succ = ZooKeeperUtil.setData(zookeeper, path, data);
		if (succ) {
			System.out.println("Success!");
		} else {
			System.out.println("Failed!");
		}
	}
	
}
