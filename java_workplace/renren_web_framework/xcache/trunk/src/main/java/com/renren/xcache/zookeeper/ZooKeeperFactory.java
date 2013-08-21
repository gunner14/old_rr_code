package com.renren.xcache.zookeeper;

import java.io.IOException;
import java.util.List;

import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;

import com.renren.xcache.config.ConfigManager;

/**
 * 统一封装获取ZooKeeper实例的逻辑
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:07:09
 */
public class ZooKeeperFactory {

	private static final String CONNECT_STRING = loadConnectString();
	
	private static final int DEFUALT_ZOOKEEPER_SESSION_TIMEOUT = 30000;
	
	public static ZooKeeper newZooKeeper(Watcher watcher) {
		return newZooKeeper(CONNECT_STRING, watcher);
	}
	
	public static ZooKeeper newZooKeeper(String connectString, Watcher watcher) {
		try {
			ZooKeeper zookeeper = new ZooKeeper(connectString,
					DEFUALT_ZOOKEEPER_SESSION_TIMEOUT, watcher);
			return zookeeper;
		} catch (IOException e) {
			throw new RuntimeException("Error occurs while creating ZooKeeper instance.", e);
		}
	}
	
	private static String loadConnectString() {
		
		//load hosts
		List<String> servers = ConfigManager.getInstance().getZkServers();
		if(servers.size() == 0) {
			throw new RuntimeException("zookeeper server config not found.");
		}
		StringBuilder sb = new StringBuilder();
		for (String server : servers) {
			sb.append(server);
			sb.append(",");
		}
		sb.setLength(sb.length() - 1);	//remove trailing ','
		
		String rootPath = ConfigManager.getInstance().getConfigRootPath();
		if (rootPath == null) {
			throw new RuntimeException("zookeeper root path config  not found.");
		}
		sb.append(rootPath);
		return sb.toString();
	}
	
	/*private static String loadHostsConfig() {
		BufferedReader reader = null;
		try {
			InputStream is = ZooKeeperFactory.class.getResourceAsStream("zookeeper-hosts.conf");
			reader = new BufferedReader(new InputStreamReader(is));
			String line;
			StringBuilder sb = new StringBuilder();
			while ((line = reader.readLine()) != null) {
				line = line.trim();
				if (line.length() == 0 || line.startsWith("#")) {
					//do nothing
				} else {
					sb.append(line);
					sb.append(',');
				}
			}
			
			if (sb.charAt(sb.length() - 1) == ',') {
				sb.setLength(sb.length() - 1);
			}
			return sb.toString();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return null;
	}*/
	
	public static void main(String[] args) {
		System.out.println(CONNECT_STRING);
	}
	
}
