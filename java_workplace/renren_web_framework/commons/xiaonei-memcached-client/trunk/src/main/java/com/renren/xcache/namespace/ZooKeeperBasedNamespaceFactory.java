package com.renren.xcache.namespace;

import java.io.ByteArrayInputStream;

import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException.NoNodeException;

import com.renren.xcache.zookeeper.ZooKeeperFactory;

/**
 * 基于ZooKeeper配置服务的{@link NamespaceFactory}实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:41:11
 */
public class ZooKeeperBasedNamespaceFactory extends AbstractNamespaceFactory {

	private static final ZooKeeperBasedNamespaceFactory instance = new ZooKeeperBasedNamespaceFactory();
	
	public static ZooKeeperBasedNamespaceFactory getInstance(){
		return instance;
	} 
	
	public static final String ROOT_PATH = "/namespaces/";
	
	public static final String EXT = ".ns";
	
	private ZooKeeper zookeeper;
	
	private Watcher zookeeperWatcher = new ZookeeperWatcher();

	private ZooKeeperBasedNamespaceFactory() {
		init();
	}
	
	@Override
	public Namespace loadConfig(String name){
		try {
			byte[] data = zookeeper.getData(getConfigPath(name), zookeeperWatcher, null);
			return NamespaceUtil.loadFromConfig(name, new ByteArrayInputStream(data));
		} catch(NoNodeException e) {
			return null;
		} catch (Exception e) {
			throw new RuntimeException(
					"Error occurs while loading namespace config: " + name, e);
		} 
	}

	private String getConfigPath(String poolName){
		return ROOT_PATH + poolName + EXT;
	}
	
	public void init() {
		zookeeper = ZooKeeperFactory.newZooKeeper(zookeeperWatcher);
	}
	
	private class ZookeeperWatcher implements Watcher {
		@Override
		public void process(WatchedEvent event) {
			
			String path = event.getPath();
	        if (event.getType() == Event.EventType.None) {
	            // We are are being told that the state of the
	            // connection has changed
	            switch (event.getState()) {
	            case SyncConnected:
	                // In this particular example we don't need to do anything
	                // here - watches are automatically re-registered with 
	                // server and any watches triggered while the client was 
	                // disconnected will be delivered (in order of course)
	                break;
	            case Expired:
	            	logger.error("Zookeeper session expired:" + event);
	            	ZooKeeper old = zookeeper;
	            	if (logger.isInfoEnabled()) {
	            		logger.info("re-initializing ZooKeeper");
	            	}
	            	init();	//重新初始化一下，也就是重新连接一下
	            	if (logger.isInfoEnabled()) {
	            		logger.info("closing expired ZooKeeper:" + old.hashCode());
	            	}
	            	try {
						old.close();	//将旧的关闭掉
					} catch (InterruptedException e) {
						e.printStackTrace();
					}	
	                break;
	            }
	        } else if (event.getType() == Event.EventType.NodeChildrenChanged) {
	        	//do nothing
	        } else if (event.getType() == Event.EventType.NodeDeleted) {
	        	/*if (logger.isInfoEnabled()) {
	        		logger.info("node deleted:" + path);
	        	}*/
	        	
	        	//do nothing
	        } else if (event.getType() == Event.EventType.NodeDataChanged) {
	        	int index = path.indexOf(ROOT_PATH);
	        	if (index >= 0) {
	        		String fileName = path.substring(ROOT_PATH.length());
	        		if (fileName.endsWith(EXT)) {
	        			String name = fileName.substring(0, fileName.length() - EXT.length());
	        			Namespace newNamespace = loadConfig(name);
	        			onNamespaceModified(newNamespace);
	        		} else {
	        			logger.warn("Unrecognized fileName:" + event);
	        		}
	        	} else {
	        		logger.warn("Unrecognized event:" + event);
	        	}
	        	
	        } else {
	        	logger.warn("Unhandled event:" + event);
	        }
		}
	} 
	
}
