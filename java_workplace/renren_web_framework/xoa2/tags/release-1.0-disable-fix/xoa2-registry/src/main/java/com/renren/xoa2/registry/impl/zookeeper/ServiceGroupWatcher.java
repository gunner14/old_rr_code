package com.renren.xoa2.registry.impl.zookeeper;

import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;

import com.renren.xoa2.registry.Shard;
import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.impl.ServiceGroupImpl;

public class ServiceGroupWatcher extends ServiceGroupImpl implements Watcher {
    protected final static Log logger = LogFactory
            .getLog(ServiceGroupWatcher.class);

    private String parentPath = null;
    private String groupName = null;
    private ZooKeeper zookeeper = null;

    private ServiceWatcher serviceWatcher = null;

    ServiceGroupWatcher(ZooKeeper zookeeper, String parentPath, String groupName) {
        this.zookeeper = zookeeper;
        this.parentPath = parentPath;
        this.groupName = groupName;
    }

    ServiceGroupWatcher(ServiceWatcher serviceWatcher, ZooKeeper zookeeper,
            String parentPath, String groupName) {
        this.serviceWatcher = serviceWatcher;
        this.zookeeper = zookeeper;
        this.parentPath = parentPath;
        this.groupName = groupName;
    }

    int propGetPriority(Properties prop) {
        int priority = ServiceGroup.DEFAULT_PRIORITY;
        String priorityConfig = prop
                .getProperty(ConfigConstants.PROPERTY_KEY_PRIORITY);
        if (priorityConfig != null) {
            try {
                priority = Integer.parseInt(priorityConfig);
            } catch (NumberFormatException e) {
                logger.error("Invalid group priority configuration: "
                        + priorityConfig, e);
            }
        }
        return priority;
    }

    int propGetShardFactor(Properties prop) {
        int shardFactor = ServiceGroup.DEFAULT_SHARD_FACTOR;
        String shardFactorConfig = prop
                .getProperty(ConfigConstants.PROPERTY_KEY_SHARD_FACTOR);
        if (shardFactorConfig != null) {
            try {
                shardFactor = Integer.parseInt(shardFactorConfig);
                if (shardFactor <= 0) {
                    shardFactor = 1;
                }
            } catch (NumberFormatException e) {
                logger.error("Invalid group shard_factor configuration: "
                        + shardFactorConfig, e);
            }
        }
        return shardFactor;
    }

    boolean load() {
        int groupId = 0;
        try {
            groupId = Integer.parseInt(groupName
                    .substring(ConfigConstants.SERVICE_GROUP_PREFIX.length()));
        } catch (NumberFormatException e) {
            return false;
        }
        setId(groupId);

        String fullPath = parentPath + '/' + groupName;

        byte[] data;
        try {
            data = zookeeper.getData(fullPath, this, null);
        } catch (KeeperException e) {
            e.printStackTrace();
            return false;
        } catch (InterruptedException e) {
            e.printStackTrace();
            return false;
        }
        Properties prop = PathUtil.dataToProperties(data);

        setPriority(propGetPriority(prop));
        setShardFactor(propGetShardFactor(prop));

        List<String> children = null;
        try {
            children = zookeeper.getChildren(fullPath, this);
        } catch (KeeperException.NoNodeException ex) {
            ex.printStackTrace();
            return false;
        } catch (KeeperException ex) {
            ex.printStackTrace();
            return false;
        } catch (InterruptedException ex) {
            ex.printStackTrace();
            return false;
        }

        List<Shard> shardList = new ArrayList<Shard>();
        for (String child : children) {
            if (child.startsWith(ConfigConstants.SHARD_PREFIX)) {
                try {
                    ShardWatcher shard = new ShardWatcher(zookeeper, fullPath,
                            child);
                    int shardId = Integer.parseInt(child
                            .substring(ConfigConstants.SHARD_PREFIX.length()));
                    shard.setId(shardId);

                    if (shard.load()) {
                        shardList.add(shard);
                    }
                } catch (NumberFormatException e) {
                    logger.error("Invalid shard name:" + fullPath + "/" + child);
                }
            }
        }

        // TODO : thread safety
        setShards(shardList);
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
        } else if (event.getType() == Event.EventType.NodeDeleted) {
            logger.info("node deleted: " + path);

            // 删除的事件可以不用处理，因为如果是某个节点的配置被删除的话，
            // 同时还会触发Event.EventType.NodeDataChanged事件，所以
            // 由那个分支去处理就可以了

        } else if (event.getType() == Event.EventType.NodeDataChanged) {
            String fullPath = parentPath + '/' + groupName;
            byte[] data = null;
            try {
                data = zookeeper.getData(fullPath, this, null);
            } catch (KeeperException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            Properties prop = PathUtil.dataToProperties(data);
            int priority = propGetPriority(prop);
            if (priority != getPriority()) {
                logger.warn("serviceGroup " + fullPath + " priority changed : "
                        + getPriority() + "->" + priority);
                setPriority(priority);
                if (serviceWatcher != null)
                    serviceWatcher.childPriorityChange(this);
            }

            int shardFactor = propGetShardFactor(prop);
            if (shardFactor != getShardFactor()) {
                logger.warn("serviceGroup " + fullPath
                        + " shardFactor changed : " + getShardFactor() + "->"
                        + shardFactor);
                // TODO: 修改shard factor 是很危险的操作
                setShardFactor(shardFactor);
            }
        } else {
            logger.warn("Unhandled event:" + event);
        }
    }
}
