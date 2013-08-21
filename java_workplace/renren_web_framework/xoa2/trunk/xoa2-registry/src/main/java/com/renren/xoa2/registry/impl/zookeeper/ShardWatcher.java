package com.renren.xoa2.registry.impl.zookeeper;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.impl.ShardImpl;

public class ShardWatcher extends ShardImpl implements Watcher {
	protected final static Log logger = LogFactory.getLog(ShardWatcher.class);
	private ZooKeeper zookeeper = null;
	private String parentPath = null;
	private String shardName = null;

	public ShardWatcher(ZooKeeper zookeeper, String parentPath, String shardName) {
		this.zookeeper = zookeeper;
		this.parentPath = parentPath;
		this.shardName = shardName;
	}
	
	public boolean load() {
		int shardId = -1;
        if (shardName.startsWith(ConfigConstants.SHARD_PREFIX)) {
            try {
                shardId = Integer.parseInt(shardName.substring(ConfigConstants.SHARD_PREFIX.length()));                
            } catch (NumberFormatException e) {
                logger.error("Invalid shard config file name:" + parentPath + "/" + shardName);
                return false;
            }
        }
		String fullPath = parentPath + '/' + shardName;		
        List<String> children = null;
		try {
			children = zookeeper.getChildren(fullPath, this);
		} catch (KeeperException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}
        
        List<Node> nodes = new ArrayList<Node>();
        for (String child : children) {
        	NodeWatcher node = new NodeWatcher(zookeeper, fullPath, child);
        	if (node.load()) {
               nodes.add(node);
           } else {
               logger.warn("Unrecognized znode:" + fullPath + "/" + child);
            }
        }
        setNodes(nodes);
        setId(shardId);
        return true;
    }
    
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
				break;
			case Expired:
				logger.warn("Zookeeper session expired: " + event);
				break;
			}
		} else if (event.getType() == Event.EventType.NodeChildrenChanged) {
			load();	// 重新加载子节点
		} else if (event.getType() == Event.EventType.NodeDeleted) {
			logger.info("node deleted: " + path);
		} else if (event.getType() == Event.EventType.NodeDataChanged) {
			// shard 节点暂时没有用到数据
		} else {
			logger.warn("Unhandled event:" + event);
		}
	}
}
