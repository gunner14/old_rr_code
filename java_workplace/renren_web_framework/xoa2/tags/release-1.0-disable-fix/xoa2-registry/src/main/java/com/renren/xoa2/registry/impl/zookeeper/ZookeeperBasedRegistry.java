package com.renren.xoa2.registry.impl.zookeeper;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Properties;
import java.util.regex.Pattern;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.KeeperException.ConnectionLossException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.data.Stat;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.Service;
import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.Shard;
import com.renren.xoa2.registry.impl.AbstractXoaRegistry;
import com.renren.xoa2.registry.impl.NodeImpl;
import com.renren.xoa2.registry.impl.ServiceGroupImpl;
import com.renren.xoa2.registry.impl.ServiceImpl;
import com.renren.xoa2.registry.impl.ShardImpl;

/**
 * 基于Zookeeper的registry实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-12-15 下午03:44:16
 */
public class ZookeeperBasedRegistry extends AbstractXoaRegistry {
	
    protected final Log logger = LogFactory.getLog(this.getClass());
    
	private ZooKeeper zookeeper;
	
	//private ClientCheckinService checkinService;
	
	private ZookeeperWatcher zookeeperWatcher = new ZookeeperWatcher();
	
	/**
	 * 表示是否第一次连接，第一次连接后不进行刷新数据的操作
	 */
	private boolean firstSyncConnected = true;
	
	public void init() {
		zookeeper = ZooKeeperFactory.newZooKeeper(zookeeperWatcher);
		//checkinService = new CheckinServiceImpl();
	}
	
	/**
	 * Watcher实现
	 */
	private class ZookeeperWatcher implements Watcher {
		@Override
		public void process(WatchedEvent event) {
			
		    if (logger.isDebugEnabled()) {
                logger.debug("WatchedEvent received: " + event);
            }
		    
		    //TODO: Watcher需要认真实现
		    
			String path = event.getPath();
	        if (event.getType() == Event.EventType.None) {
	            // We are are being told that the state of the
	            // connection has changed
	            switch (event.getState()) {
	            case SyncConnected:
	                if (logger.isInfoEnabled()) {
	                    logger.info("Zookeeper SyncConnected: " + event);
	                }
	                
	                
	                if (firstSyncConnected) {  //第一次连接后不刷新数据
	                    firstSyncConnected = false;
	                } else {
	                    
	                    try {
	                        refreshZookeeperData();  //刷新数据, 防止重连时可能造成的信息丢失
	                    } catch (Throwable e) {
	                        if (e instanceof ConnectionLossException
	                                || (e.getCause() != null && e.getCause() instanceof ConnectionLossException)) {
	                            
	                            //如果是ConnectionLossException引起的，重连之
	                            reInitZk(); //重连成功后会自动进入SyncConnected状态，所以会执行refreshZookeeperData()操作
	                        } else {
	                            logger.error("Error while refreshing data from ZooKeeper on event: " + event, e);
	                        }
	                    }
	                }
	                break;
	            case Expired:
	            	logger.warn("Zookeeper session expired: " + event);
	            	reInitZk();   //重连成功后会自动进入SyncConnected状态，所以会执行refreshZookeeperData()操作
	                break;
	            }
	        } else if (event.getType() == Event.EventType.NodeChildrenChanged) {
	        	/*if (path.endsWith(PathUtil.SERVICE_NODE_FOLDER)) {
					
	        		//这类事件的path应该类似: /com/renren/xoa/test/.service-nodes
	        		
	        		String serviceId = PathUtil.toServiceId(path.substring(0,
							path.length()
									- PathUtil.SERVICE_NODE_FOLDER.length()));
					if (logger.isInfoEnabled()) {
						logger.info("config changed for service: " + serviceId);
					}
					updateServiceNodes(serviceId, getServiceNodes(serviceId));
	        	} else {
	        		logger.warn("Unrecognized event:" + event);
	        	}*/
	        } else if (event.getType() == Event.EventType.NodeDeleted) {
	        	if (logger.isInfoEnabled()) {
	        		logger.info("node deleted: " + path);
	        	}
	        	
	        	//删除的事件可以不用处理，因为如果是某个节点的配置被删除的话，
	        	//同时还会触发Event.EventType.NodeDataChanged事件，所以
	        	//由那个分支去处理就可以了
	        	
	        } else if (event.getType() == Event.EventType.NodeDataChanged) {
	        	/*
	        	int index = path.indexOf(PathUtil.SERVICE_NODE_FOLDER);
	        	if (index > 0) {
	        		String serviceId = PathUtil.toServiceId(path.substring(0, index));
					if (logger.isInfoEnabled()) {
						logger.info("config changed for service: " + serviceId
								+ ", caused by node change:" + path);
					}
					updateServiceNodes(serviceId, getServiceNodes(serviceId));
	        	} else {
	        		logger.warn("Unrecognized event:" + event);
	        	}*/
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
            init(); //重新初始化一下，也就是重新连接一下
        } finally { //确保老的资源能顺利被释放
            if (logger.isInfoEnabled()) {
                logger.info("closing expired ZooKeeper:" + old.hashCode());
            }
            try {
                old.close();    //将旧的关闭掉
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
	    //更新所有的服务节点信息
	}
	
	private final Pattern ipPattern = Pattern.compile("[\\d]{1,3}(\\.[\\d]{1,3}){1,3}:[\\d]{1,5}");
	
	/**
	 * 匹配IP格式
	 * @param ip
	 * @return
	 */
	private boolean isValidIpPort(String ipPort) {
		return ipPattern.matcher(ipPort).matches();
	}
	
	/**
	 * 销毁资源
	 */
	public void destroy() {
	    try {
            zookeeper.close();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
	}

    @Override
    protected Service loadService(String serviceId) {
        
        //根据给定的serviceId拼出存储在zookeeper中的配置文件的路径
        //如serviceId = "demo.xoa.renren.com"
        String nodePath = PathUtil.serviceIdToPath(serviceId);
        
        //此时nodePath为~/com/renren/xoa/demo
        try {
            Stat nodeStat = zookeeper.exists(nodePath, false);
            if (nodeStat == null) {
                logger.error("Path not exists: " + nodePath);
                return null;
            }
            ServiceGroup activeGroup = findActiveServiceGroup(nodePath);
            if (activeGroup != null) {
                ServiceImpl service = new ServiceImpl();
                service.setId(serviceId);
                service.setActiveServiceGroup(activeGroup);
                return service;
            }
        } catch (KeeperException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return null;
    }
    
    /**
     * 从所有的group配置中找到一个active的group配置
     * @param parentPath
     * @return
     * @throws KeeperException
     * @throws InterruptedException
     */
    private ServiceGroup findActiveServiceGroup(String parentPath) throws KeeperException, InterruptedException {
        //找到serviceId路径后，寻找其所有子节点
        List<String> children = zookeeper.getChildren(parentPath, zookeeperWatcher);
        
        List<ServiceGroup> groups = new ArrayList<ServiceGroup>();
        
        //寻找.service_group_x节点
        for (String child : children) {
            if (isServiceGroup(child)) {
                ServiceGroup group = loadServiceGroup(child, parentPath);
                if (group != null) {
                    groups.add(group);
                }
            }
        }
        if (groups.size() == 0) {
            return null;
        }
        
        //从所有的group中找到active group，即优先级最高的那个
        ServiceGroup activeGroup = groups.get(0);
        
        for (ServiceGroup group : groups) {
            if (group.getPriority() > activeGroup.getPriority()) {
                activeGroup = group;
            }
        }
        return activeGroup;
    }

    /**
     * 加载一个ServiceGroup的配置信息
     * 
     * @param groupFileName
     * @param parentPath
     * @return
     * @throws KeeperException
     * @throws InterruptedException
     */
    private ServiceGroup loadServiceGroup(String groupFileName, String parentPath) throws KeeperException, InterruptedException {
        
        //从文件名.service_group_{id}中获取groupId
        int groupId = Integer.parseInt(groupFileName.substring(ConfigConstants.SERVICE_GROUP_PREFIX.length()));
        
        ServiceGroupImpl group = new ServiceGroupImpl();
        group.setId(groupId);
        
        String groupFilePath = parentPath + "/" + groupFileName;
        
        byte[] data = zookeeper.getData(groupFilePath,
                zookeeperWatcher, null);
        
        Properties prop = dataToProperties(data);
        
        int priority = ServiceGroup.DEFAULT_PRIORITY;
        String priorityConfig = prop.getProperty(ConfigConstants.PROPERTY_KEY_PRIORITY);
        if (priorityConfig != null) {
            try {
                priority = Integer.parseInt(priorityConfig);
            } catch(NumberFormatException e) {
                logger.error("Invalid group priority configuration: " + priorityConfig, e);
            }
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("group priority not configured in file: " + groupFilePath);
            }
        }
        
        int shardFactor = ServiceGroup.DEFAULT_SHARD_FACTOR;
        String shardFactorConfig = prop.getProperty(ConfigConstants.PROPERTY_KEY_SHARD_FACTOR);
        if (shardFactorConfig != null) {
            try {
                shardFactor = Integer.parseInt(shardFactorConfig);
            } catch(NumberFormatException e) {
                logger.error("Invalid group shard_factor configuration: " + shardFactorConfig, e);
            }
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("group priority not configured in file: " + groupFilePath);
            }
        }
        group.setShardFactor(shardFactor);
        group.setPriority(priority);
        List<Shard> shards = loadShards(groupFilePath);
        
        //检查一下shard配置是否够
        if (shards.isEmpty()) {
            logger.error("No shard under path: " + groupFilePath);
            return null;
        }
        group.setShards(shards);
        return group;
    }
    
    /**
     * 加载某个ServiceGroup下面的所有的Shard的配置信息
     * 
     * @param parentPath
     * @return
     * @throws InterruptedException 
     * @throws KeeperException 
     */
    private List<Shard> loadShards(String parentPath) throws KeeperException, InterruptedException {
        
        List<String> children = zookeeper.getChildren(parentPath, zookeeperWatcher);
        List<Shard> shards = new ArrayList<Shard>();
        for(String child : children) {
            if (child.startsWith(ConfigConstants.SHARD_PREFIX)) {
                try {
                    int shardId = Integer.parseInt(child.substring(ConfigConstants.SHARD_PREFIX.length()));
                    ShardImpl shard = new ShardImpl();
                    shard.setId(shardId);
                    shard.setNodes(loadEnabledNodes(parentPath + "/" + child));
                    shards.add(shard);
                } catch (NumberFormatException e) {
                    logger.error("Invalid shard config file name:" + parentPath + "/" + child);
                }
            }
        }
        
        //按shardId排序一下
        Collections.sort(shards);
        return Collections.unmodifiableList(shards);
    }

    /**
     * 加载某个Shard下的所有状态为enabled的节点信息
     * 
     * @param string
     * @return
     * @throws InterruptedException 
     * @throws KeeperException 
     */
    private List<Node> loadEnabledNodes(String parentPath) throws KeeperException, InterruptedException {
        
        List<String> children = zookeeper.getChildren(parentPath, zookeeperWatcher);
        
        List<Node> nodes = new ArrayList<Node>();
        for (String child : children) {
            if(isValidIpPort(child)) {  //child是IP:Port
                byte[] data = zookeeper.getData(parentPath + "/" + child,
                        zookeeperWatcher, null);
        
                String[] ss = child.split(":");
                String ip = ss[0];
                int port = Integer.parseInt(ss[1]);
                
                Properties prop = dataToProperties(data);
                String statusConfig = prop.getProperty(ConfigConstants.PROPERTY_NODE_STATUS);
                
                boolean disabled = true;
                if (ConfigConstants.NODE_STATUS_ENABLED.equals(statusConfig)) {
                    disabled = false;
                } else {
                  logger.warn("Skip disabled service node : " + parentPath + "/" + child);
                	continue;
                }
                
                NodeImpl node = new NodeImpl();
                node.setHost(ip);
                node.setPort(port);
                node.setDisabled(disabled);
                nodes.add(node);
            } else {
                logger.warn("Unrecognized znode:" + parentPath + "/" + child);
            }
        }
        
        //排序一下
        Collections.sort(nodes);
        
        //包成不可变更的集合
        return Collections.unmodifiableList(nodes);
    }

    private boolean isServiceGroup(String fileName) {
        if (fileName.startsWith(ConfigConstants.SERVICE_GROUP_PREFIX)) {
            String s_groupId = fileName.substring(ConfigConstants.SERVICE_GROUP_PREFIX.length());
            try {
                Integer.parseInt(s_groupId);
                return true;
            } catch (NumberFormatException e) {
                return false;
            }
        }
        return false;
    }
    
	private Properties dataToProperties(byte[] data) {
	    Properties p = new Properties();
	    try {
            p.load(new ByteArrayInputStream(data));
        } catch (IOException e) {
            e.printStackTrace();
        }
	    return p;
	}
    
}
