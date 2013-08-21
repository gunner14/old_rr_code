package com.renren.xcache.zookeeper;

import java.util.Collections;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.ZooDefs.Perms;
import org.apache.zookeeper.data.ACL;
import org.apache.zookeeper.data.Id;
import org.apache.zookeeper.data.Stat;

/**
 * ZooKeeper相关工具
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:07:49
 */
public class ZooKeeperUtil {

	public static final int NO_VERSION = -1; // -1是指不关心version

	/**
	 * 向ZooKeeper中设置数据的工具方法。
	 * 
	 * @param zookeeper
	 * @param path
	 * @param data
	 * @return
	 */
	public static boolean setData(ZooKeeper zookeeper, String path, byte[] data) {

		try {
			Stat stat = zookeeper.exists(path, false);
			if (stat == null) { // node not exists
				zookeeper.create(path, data, Collections.singletonList(new ACL(
						Perms.ALL, new Id("world", "anyone"))),
						CreateMode.PERSISTENT);
			} else { // node exists
				zookeeper.setData(path, data, NO_VERSION);
			}
			return true;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}
}
