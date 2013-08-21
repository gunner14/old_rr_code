package com.renren.xoa2.registry.impl.zookeeper;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.ZooKeeper;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.impl.ServiceImpl;

public class ServiceWatcher extends ServiceImpl implements Watcher  {
	protected final static Log logger = LogFactory.getLog(ServiceWatcher.class);

	private String parentPath = null;    
	private ZooKeeper zookeeper = null;	
	
	public ServiceWatcher(String parentPath) {
		this.parentPath = parentPath;
	}
	
	@Override
	public void process(WatchedEvent event) {
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
				zookeeper = null;
				load();
				break;
			}

		} else if (event.getType() == Event.EventType.NodeChildrenChanged) {			
			load();
		  logger.warn("child node changed. current active node : " + getActiveServiceGroup().getId());
		} else if (event.getType() == Event.EventType.NodeDeleted) {
		} else if (event.getType() == Event.EventType.NodeDataChanged) {
		} else {
			logger.warn("Unhandled event:" + event);
		}
	}

	public boolean load() {
		try {
			if (zookeeper == null) {
				zookeeper = ZooKeeperFactory.newZooKeeper(this);
			}
		} catch (Exception e) {
			return false;
		}
		
		// 找到serviceId路径后，寻找其所有子节点
		List<String> children = null;
        try {
			children = zookeeper.getChildren(parentPath, this);			
		} catch (KeeperException.NoNodeException e) {
			e.printStackTrace();
			return false;
		} catch (KeeperException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {			
			e.printStackTrace();
			return false;
		}

		setId(PathUtil.toServiceId(parentPath));
        
        List<ServiceGroup> groups = new LinkedList<ServiceGroup>();
        ServiceGroup activeGroup = null;
        //寻找.service_group_x节点
       for (String child : children) {
            if (isServiceGroup(child)) {
                ServiceGroupWatcher group = new ServiceGroupWatcher(this, zookeeper, parentPath, child);
                if (group.load()) {
                    groups.add(group);
                    if (activeGroup == null || group.getPriority() > activeGroup.getPriority()) {
                    	  activeGroup = group;
                       }
                   }
              }
        }
       setActiveServiceGroup(activeGroup);
       return true;
	}
	
  void childPriorityChange(ServiceGroupWatcher group) {
    // 值考虑"设为最高优先级"的情况
    if (group.getPriority() > getActiveServiceGroup().getPriority()) {
    	setActiveServiceGroup(group);
    }
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

	public boolean registerNode(int groupId, int shardId, Node node) {
		try {
			if (zookeeper == null) {
				zookeeper = ZooKeeperFactory.newZooKeeper(this);
			}
		} catch (Exception e) {
			return false;
		}
		
		List<String> pathList = new ArrayList<String>();
		
		int pos = parentPath.lastIndexOf('/');
		pathList.add(parentPath.substring(0, pos));
		pathList.add(parentPath);
		String groupPath = parentPath + '/' + ConfigConstants.SERVICE_GROUP_PREFIX + groupId;
		pathList.add(groupPath);

		String shardPath = groupPath + '/' + ConfigConstants.SHARD_PREFIX + shardId;
		pathList.add(shardPath);
		
		for (String path : pathList) {
	       try {
				zookeeper.create(path, "".getBytes(), Ids.OPEN_ACL_UNSAFE, 
						CreateMode.PERSISTENT);
			} catch (KeeperException.NodeExistsException e) {
				// 正常情况
			} catch (KeeperException e) {
				e.printStackTrace();
				return false;
			} catch (InterruptedException e) {			
				e.printStackTrace();
				return false;
			}
		}
		
		StringBuilder leafData = new StringBuilder();
		leafData.append("status=");		
		leafData.append(node.isDisabled() ? "disabled" : "enabled");
		
		String leafPath = shardPath + '/' + node.getHost() + ':' + node.getPort();		
		
       try {
			zookeeper.create(leafPath, leafData.toString().getBytes(), Ids.OPEN_ACL_UNSAFE, 
					CreateMode.EPHEMERAL);
		} catch (KeeperException.NodeExistsException e) {
			// 正常情况
		} catch (KeeperException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {			
			e.printStackTrace();
			return false;
		}
		
		return true;
	}

	public boolean disableNode(int groupId, int shardId, Node node) {
		try {
			if (zookeeper == null) {
				zookeeper = ZooKeeperFactory.newZooKeeper(this);
			}
		} catch (Exception e) {
			return false;
		}
		
		String data = "status=disabled";

		StringBuilder path = new StringBuilder();
		path.append(parentPath).append('/')
			.append(ConfigConstants.SERVICE_GROUP_PREFIX).append(groupId).append('/')
			.append(ConfigConstants.SHARD_PREFIX).append(shardId).append('/')
			.append(node.getHost()).append(':').append(node.getPort());		
		
       try {
			zookeeper.setData(path.toString(), data.getBytes(), -1);
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
