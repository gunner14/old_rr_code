package com.renren.xoa2.registry.impl.zookeeper;

import java.util.Properties;
import java.util.regex.Pattern;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;

import com.renren.xoa2.registry.impl.NodeImpl;

public class NodeWatcher extends NodeImpl implements Watcher {
    protected final static Log logger = LogFactory.getLog(NodeWatcher.class);

    private ZooKeeper zookeeper = null;
    private String parentPath = null;
    private String nodeName = null;

    public NodeWatcher(ZooKeeper zk, String parentPath, String nodeName) {
        zookeeper = zk;
        this.parentPath = parentPath;
        this.nodeName = nodeName;
    }

    private final Pattern ipPattern = Pattern
            .compile("[\\d]{1,3}(\\.[\\d]{1,3}){1,3}:[\\d]{1,5}");

    private boolean isValidIpPort(String ipPort) {
        return ipPattern.matcher(ipPort).matches();
    }

    private Properties loadProperties() {
        byte[] data = null;

        try {
            data = zookeeper.getData(parentPath + "/" + nodeName, this, null);
        } catch (KeeperException.NoNodeException e) {
            logger.warn("znode " + parentPath + "/" + nodeName + " not exist.");
            return null;
        } catch (KeeperException e) {
            e.printStackTrace();
            return null;
        } catch (InterruptedException e) {
            e.printStackTrace();
            return null;
        }
        System.out.println("node " + parentPath + "/" + nodeName + 
                " data=" + new String(data));
        return PathUtil.dataToProperties(data);
    }

    private static boolean propIsDisabled(Properties prop) {
        if (prop != null) {
            String statusConfig = prop
                    .getProperty(ConfigConstants.PROPERTY_NODE_STATUS);

            if (statusConfig != null
                    && ConfigConstants.NODE_STATUS_ENABLED.equals(statusConfig)) {
                return false;
            }
        }
        return true;
    }

    public boolean load() {
        if (!isValidIpPort(nodeName)) { // child是IP:Port
            return false;
        }
        System.out.println("load node : " + nodeName);

        String[] ss = nodeName.split(":");
        String ip = ss[0];
        int port = Integer.parseInt(ss[1]);
        Properties prop = loadProperties();
        if (prop == null) {
            return false;
        }

        setHost(ip);
        setPort(port);
        setDisabled(propIsDisabled(prop));
        setHealthy(true);
        return true;
    }

    @Override
    public void process(WatchedEvent event) {
        if (logger.isDebugEnabled()) {
            logger.debug("WatchedEvent received: " + event);
        }
        System.out.println("WatchedEvent received: " + event);

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
        } else if (event.getType() == Event.EventType.NodeDeleted) {
            // 此处不做处理，父节点(shard)会处理该事件
            logger.info("node deleted: " + path);
        } else if (event.getType() == Event.EventType.NodeDataChanged) {
            Properties prop = loadProperties();
            if (prop == null) {
                // Event.EventType.NodeDataChanged事件 node 被删除的情况, 交给Shard处理
                setDisabled(true);
            } else {
                setDisabled(propIsDisabled(prop));
            }
        } else {
            logger.warn("Unhandled event:" + event);
        }
    }
}
