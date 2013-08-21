package com.renren.xoa2.registry.impl.zookeeper;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.Properties;

import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.ZooDefs.Ids;

/**
 * 路径计算相关的工具类
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-20 上午11:21:12
 */
public class PathUtil {

	
	/**
	 * 给定serviceId，计算出其对应的path，相对路径
	 * @param serviceId
	 * @return
	 */
	public static String serviceIdToPath(String serviceId) {
		String[] ss = serviceId.split("\\.");
		StringBuilder sb = new StringBuilder(50);
		for(int i = ss.length - 1; i >= 0; i--) {
			sb.append("/");
			sb.append(ss[i]);
		}
		return sb.toString();
	}
	
	/**
	 * 给定相对的path，计算出serviceId
	 * 
	 * @param znodePath
	 * @return
	 */
	public static String toServiceId(String znodePath) {
		String[] ss = znodePath.split("/");
		StringBuilder sb = new StringBuilder(znodePath.length());
		for(int i = ss.length - 1; i >= 0; i--) {
			if (ss[i].length() > 0) {
				sb.append(ss[i]);
				sb.append(".");
			}
		}
		if (sb.charAt(sb.length() - 1) == '.') {
			sb.deleteCharAt(sb.length() - 1);
		}
		return sb.toString();
	}

	/**
	 * 给定数据，生成相应的Properties对象
	 * 
	 */
	public static Properties dataToProperties(byte[] data) {
	    Properties p = new Properties();
	    try {
            p.load(new ByteArrayInputStream(data));
        } catch (IOException e) {
            e.printStackTrace();
        }
	    return p;
	}
	
	public static boolean createTempZode(ZooKeeper zk, String path, byte [] data) {
		try {
			zk.create(path, data, Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
			// zk.create(path, data, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
		} catch (KeeperException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();			
			return false;
		}
		return true;
	}

	public static boolean removeZode(ZooKeeper zk, String path) {
		try {
			zk.delete(path, -1);
		} catch (KeeperException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public static boolean setZodeData(ZooKeeper zk, String path, byte [] data) {
		try {
			zk.setData(path, data, -1);
		} catch (KeeperException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();			
			return false;
		}
		return true;
	}
}
