package com.renren.xcache.zookeeper;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;

import org.apache.zookeeper.ZooKeeper;

import com.renren.xcache.namespace.Namespace;
import com.renren.xcache.namespace.NamespaceUtil;
import com.renren.xcache.namespace.ZooKeeperBasedNamespaceFactory;

/**
 * 向ZooKeeper中同步{@link Namespace}配置的工具
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-20 下午06:55:19
 */
public class NamespaceConfigMgr {

	
	public static void main(String[] args) throws Exception {
		
		if (args.length != 1) {
			System.err.println("Usage:java NamespaceConfigMgr local_config_file_path");
			return;
		}
		String configFilePath = args[0];
		
		File file = new File(configFilePath);
		String fileName = file.getName();
		if (!fileName.endsWith(ZooKeeperBasedNamespaceFactory.EXT)) {
			throw new IllegalArgumentException("File name must be *."
					+ ZooKeeperBasedNamespaceFactory.EXT + ", but yours:" + fileName);
		}
		String name = fileName.substring(0, fileName.length()
				- ZooKeeperBasedNamespaceFactory.EXT.length());
		
		FileInputStream fin = new FileInputStream(configFilePath);
		
		ByteArrayOutputStream bo = new ByteArrayOutputStream();
		byte[] buff = new byte[256];
		
		int bytesRead;
		while ((bytesRead = fin.read(buff)) > 0) {
			bo.write(buff, 0, bytesRead);
		}
		byte[] data = bo.toByteArray();
		
		System.out.println("config for namespace '" + name + "':");
		System.out.println(new String(data, "UTF-8"));
		//CachePool pool = CachePoolUtil.loadFromConfig(name, new ByteArrayInputStream(data));
		Namespace namespace = NamespaceUtil.loadFromConfig(name, new ByteArrayInputStream(data));
		System.out.println(namespace.toString());
		
		ZooKeeper zookeeper = ZooKeeperFactory.newZooKeeper(null);
		
		String path = ZooKeeperBasedNamespaceFactory.ROOT_PATH + name + ZooKeeperBasedNamespaceFactory.EXT;
		System.out.println("set data to:" + path);
		boolean succ = ZooKeeperUtil.setData(zookeeper, path, data);
		if (succ) {
			System.out.println("Success!");
		} else {
			System.out.println("Failed!");
		}
	}
	
}
