package com.renren.xoa2.registry.impl.zookeeper;

import java.util.Date;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;

public class XmqRegistry implements Watcher {
    protected final static Log logger = LogFactory.getLog(ServiceWatcher.class);
    
    private ZooKeeper zookeeper = null;
    final String ZOOKEEPER_ADDR = "10.11.21.185:2181/test";

    private ConcurrentMap<String, Long> loadTimestamps_ = new ConcurrentHashMap<String, Long>();
    private ConcurrentMap<String, List<String>> endpoints_ = new ConcurrentHashMap<String, List<String>>();
    
    private boolean initialized_ = false;

    
    public XmqRegistry() {
        initZookeeper();
    }
    
    private String getParentPath(String xmqName, XmqEndpointType type) {
        StringBuilder b = new StringBuilder("/");
        b.append(xmqName).append("/");
        if (type == XmqEndpointType.GATEWAY) {
            b.append("GateWay");
        } else {
            b.append("Writer");
        }
        
        return b.toString();
    }
    
    public String getEndpoint(String xmqName, XmqEndpointType type) {
        String path = getParentPath(xmqName, type); 
        List<String> endpoints = endpoints_.get(path);
        
        if (endpoints != null && endpoints.size() > 0)
            return endpoints.get(0);        
        
        Long lastLoadTime = loadTimestamps_.get(path);
        if (lastLoadTime == null 
                || (new Date().getTime() - lastLoadTime) > 30 * 1000) {
            // 30s之内，不重查zk
            endpoints = loadEndpoints(path);
            if (endpoints != null && endpoints.size() > 0)
                return endpoints.get(0);
        }            
        
        return null;
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

                if (!initialized_) {
                    reload();
                    initialized_ = true;
                }
                break;
            case Expired:
                logger.warn("Zookeeper session expired: " + event);
                zookeeper = null;
                initZookeeper();
                break;
            }

        } else if (event.getType() == Event.EventType.NodeChildrenChanged) {
            loadEndpoints(event.getPath());
            logger.warn("child node changed. current active node");
        } else if (event.getType() == Event.EventType.NodeDeleted) {
        } else if (event.getType() == Event.EventType.NodeDataChanged) {            
        } else {
            logger.warn("Unhandled event:" + event);
        }
        
    }
    
    private List<String> getChildren(String path) {
        try {
            return zookeeper.getChildren(path, this);
        } catch (KeeperException.NoNodeException e) {
            e.printStackTrace();
            return null;
        } catch (KeeperException e) {
            e.printStackTrace();
            return null;
        } catch (InterruptedException e) {
            e.printStackTrace();
            return null;
        }        
    }

    private boolean initZookeeper() {
        try {
            if (zookeeper == null) {
                initialized_ = false;
                zookeeper = ZooKeeperFactory.newZooKeeper(ZOOKEEPER_ADDR, this);                
            }
        } catch (Exception e) {
            logger.error("zookeeper connect error.");
            return false;
        }

        return true;
    }
    
    private void reload() {
        // 重新加载全部。expire的时候，确保老的监听重新生效
        for (String path : endpoints_.keySet()) {
            loadEndpoints(path);
        }
    }
    
    private List<String> loadEndpoints(String path) {
        if (path != null) {
            // 找到serviceId路径后，寻找其所有子节点
            List<String> children = getChildren(path);
            loadTimestamps_.put(path, new Date().getTime());
            if (children != null) {
                endpoints_.put(path, children);
                return children;
            }
        }
        return null;
    }
}
