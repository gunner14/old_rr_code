package com.renren.xcache.pool;

import java.io.ByteArrayInputStream;

import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException.ConnectionLossException;
import org.apache.zookeeper.KeeperException.NoNodeException;

import com.renren.xcache.zookeeper.ZooKeeperFactory;

/**
 * 基于ZooKeeper配置服务的{@link CachePoolFactory}实现。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:18:36
 */
public class ZooKeeperBasedPoolFactory extends AbstractPoolFactory {

	public static final String ROOT_PATH = "/pools/";
	
	public static final String EXT = ".pl";
	
	private ZooKeeper zookeeper;
	
	private Watcher zookeeperWatcher = new ZookeeperWatcher();
	
	private static ZooKeeperBasedPoolFactory instance = new ZooKeeperBasedPoolFactory();
	
	public static ZooKeeperBasedPoolFactory getInstance() {
		return instance;
	}

	private ZooKeeperBasedPoolFactory() {
		init();
	}
	
	@Override
	protected CachePool loadPoolConfig(String poolName) {
		try {
			byte[] data = zookeeper.getData(getConfigPath(poolName), zookeeperWatcher, null);
			return CachePoolUtil.loadFromConfig(poolName, new ByteArrayInputStream(data));
		} catch(NoNodeException e) {
			return null;
		} catch (Exception e) {
			throw new RuntimeException(
					"Error occurs while loading pool config: " + poolName, e);
		} 
	}

	private String getConfigPath(String poolName){
		return ROOT_PATH + poolName + EXT;
	}
	
	private void init() {
		zookeeper = ZooKeeperFactory.newZooKeeper(zookeeperWatcher);
	}
	
	private class ZookeeperWatcher implements Watcher {
		@Override
		public void process(WatchedEvent event) {
			
			if (logger.isDebugEnabled()) {
				logger.debug("WatchedEvent received: " + event);
			}
			
			String path = event.getPath();
	        if (event.getType() == Event.EventType.None) {
	            // We are are being told that the state of the
	            // connection has changed
	            switch (event.getState()) {
	            case SyncConnected:
	            	if (logger.isInfoEnabled()) {
	            		logger.info("Zookeeper SyncConnected: " + event);
	            	}
	            	try {
						refreshZookeeperData(); // 刷新数据
					} catch (Exception e) {
                        if (e instanceof ConnectionLossException
                                || (e.getCause() != null && e.getCause() instanceof ConnectionLossException)) {
                        	
                        	//如果是ConnectionLossException引起的，重连之
                        	reInitZk();	//重连成功后会自动进入SyncConnected状态，所以会执行refreshZookeeperData()操作
                        } else {
                            logger.error("Error while refreshing data from ZooKeeper on event: " + event, e);
                        }
					}
	                break;
	            case Expired:
	            	logger.error("Zookeeper session expired:" + event);
	            	reInitZk();	//重连成功后会自动进入SyncConnected状态，所以会执行refreshZookeeperData()操作
	                break;
	            }
	        } else if (event.getType() == Event.EventType.NodeChildrenChanged) {
	        	//do nothing
	        } else if (event.getType() == Event.EventType.NodeDeleted) {
	        	
	        	//do nothing
	        } else if (event.getType() == Event.EventType.NodeDataChanged) {

	        	int index = path.indexOf(ROOT_PATH);
	        	if (index >= 0) {
	        		String fileName = path.substring(ROOT_PATH.length());
	        		if (fileName.endsWith(EXT)) {
	        			String poolName = fileName.substring(0, fileName.length() - EXT.length());
	        			CachePool newPool = loadPoolConfig(poolName);
	        			onPoolModified(newPool);
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
	
	/**
	 * 初始化一个新的Zookeeper实例，销毁掉老的
	 */
	private void reInitZk() {
		ZooKeeper old = zookeeper;
    	try {
    		if (logger.isInfoEnabled()) {
        		logger.info("re-initializing ZooKeeper");
        	}
        	init();	//重新初始化一下，也就是重新连接一下
		} finally {	//确保老的资源能顺利被释放
			if (logger.isInfoEnabled()) {
        		logger.info("closing expired ZooKeeper:" + old.hashCode());
        	}
        	try {
				old.close();	//将旧的关闭掉
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 刷新数据, 防止重连时可能造成的信息丢失
	 * 
	 * 刷新数据的时候会重新注册Watcher，也能解决Watcher丢失的问题
	 */
	private void refreshZookeeperData() {
		for (String name : getPoolNames()) {
			if (logger.isInfoEnabled()) {
	            logger.info("Refreshing config for pool: " + name);
	        }
			onPoolModified(loadPoolConfig(name));
		}
	}
	
}
