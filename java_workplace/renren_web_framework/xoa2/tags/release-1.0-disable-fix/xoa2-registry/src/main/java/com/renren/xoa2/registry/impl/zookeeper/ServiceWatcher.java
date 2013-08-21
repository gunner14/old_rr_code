package com.renren.xoa2.registry.impl.zookeeper;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;

import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.impl.ServiceImpl;

public class ServiceWatcher extends ServiceImpl implements Watcher {
    protected final static Log logger = LogFactory.getLog(ServiceWatcher.class);

    private String parentPath = null;
    private ZooKeeper zookeeper = null;

    private static final List<String> DEFAULT_PHONE_LIST = new ArrayList<String>();
    static {
        DEFAULT_PHONE_LIST.add("18611951882");
    }
    
    class ReloadRunnable implements Runnable {
        private ServiceWatcher sw_;
        public ReloadRunnable(ServiceWatcher sw) {
            this.sw_ = sw;
        }
        public void run() {
            sw_.load();
        }        
    }
    
    ScheduledThreadPoolExecutor reloadExecutor = 
        new ScheduledThreadPoolExecutor(2);
    public ServiceWatcher(String parentPath) {
        this.parentPath = parentPath;
        this.setAlarmPhones(DEFAULT_PHONE_LIST);

        reloadExecutor.scheduleAtFixedRate(new ReloadRunnable(this), 300, 300, TimeUnit.SECONDS);
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
                load();
                break;
            }

        } else if (event.getType() == Event.EventType.NodeChildrenChanged) {
            load();
            logger.warn("child node changed. current active node : "
                    + getActiveServiceGroup().getId());
        } else if (event.getType() == Event.EventType.NodeDeleted) {
        } else if (event.getType() == Event.EventType.NodeDataChanged) {
            loadDataProperties();
        } else {
            logger.warn("Unhandled event:" + event);
        }
    }

    private List<String> propGetAlarmPhones(Properties prop) {
        List<String> phones = new ArrayList<String>();
        String phonesProp = prop
                .getProperty(ConfigConstants.PROPERTY_SERVICE_ALARM_PHONES);
        if (phonesProp != null) {
            String[] phoneArray = phonesProp.split(",");
    
            for (String phone : phoneArray) {
                if (phone.length() > 10) {
                    phones.add(phone);
                    System.out.println("add alarm phone:" + phone);
                }
            }
        }
        if (phones.isEmpty()) {
            System.out.println("no alarm phone");
            return DEFAULT_PHONE_LIST;
        }

        return phones;
    }

    private boolean loadDataProperties() {
        byte[] data;
        try {
            data = zookeeper.getData(parentPath, this, null);
        } catch (KeeperException e) {
            e.printStackTrace();
            return false;
        } catch (InterruptedException e) {
            e.printStackTrace();
            return false;
        }
        Properties prop = PathUtil.dataToProperties(data);
        setAlarmPhones(propGetAlarmPhones(prop));
        return true;
    }

    public boolean load() {
        try {
            if (zookeeper == null) {
                zookeeper = ZooKeeperFactory.newZooKeeper(this);
            }
        } catch (Exception e) {
            logger.error("zookeeper connect error.");
            return false;
        }

        if (!loadDataProperties()) {
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
        // 寻找.service_group_x节点
        for (String child : children) {
            if (isServiceGroup(child)) {
                ServiceGroupWatcher group = new ServiceGroupWatcher(this,
                        zookeeper, parentPath, child);
                if (group.load()) {
                    groups.add(group);
                    if (activeGroup == null
                            || group.getPriority() > activeGroup.getPriority()) {
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
            String s_groupId = fileName
                    .substring(ConfigConstants.SERVICE_GROUP_PREFIX.length());
            try {
                Integer.parseInt(s_groupId);
                return true;
            } catch (NumberFormatException e) {
                return false;
            }
        }
        return false;
    }
}
