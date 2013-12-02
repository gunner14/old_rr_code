package xce.tripod;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException.NodeExistsException;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.ZooKeeper;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class ZkWrapper {

    private static Map<String, ZooKeeper> zkMap = new HashMap<String, ZooKeeper>();

    public static ZooKeeper getInstance(String zkAddress) throws IOException {
        ZooKeeper zk = zkMap.get(zkAddress);
        if (zk == null) {
            zk = new ZooKeeper(zkAddress, 2000, null);
            zkMap.put(zkAddress, zk);
        }
        return zk;
    }

    static void dumpToXml(String zkAddress, String xmlPath) throws Exception {
        DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = null;
        builder = docFactory.newDocumentBuilder();
        Document doc = null;
        doc = builder.newDocument();
        Element root = doc.createElement("tripod");
        doc.appendChild(root);
        ZooKeeper zk = ZkWrapper.getInstance(zkAddress);

        Element cache = doc.createElement("cache");
        root.appendChild(cache);
        List<String> ccNamespaces = zk.getChildren(Constants.DEFAULT_ZK_CACHE_ZNODE, false);
        for (String namespace : ccNamespaces) {
            Element ns = doc.createElement("namespace");
            ns.setAttribute("id", namespace);
            cache.appendChild(ns);
            List<String> cacheList = zk
                    .getChildren(getZkPath(ConfigEntry.CC, namespace, ""), false);
            if (cacheList != null) {
                for (String server : cacheList) {
                    Element node = doc.createElement("node");
                    String[] units = ConfigEntry.checkCacheServerProcess(server);
                    node.setAttribute("nickname", units[0]);
                    node.setAttribute("address", units[1]);
                    node.setAttribute("port", units[2]);
                    ns.appendChild(node);
                }
            }
        }

        Element cm = doc.createElement("cachemanager");
        root.appendChild(cm);
        List<String> cmNamespaces = zk.getChildren(Constants.DEFAULT_ZK_CACHEMANAGERZNODE, false);
        for (String namespace : cmNamespaces) {
            Element ns = doc.createElement("namespace");
            ns.setAttribute("id", namespace);
            cm.appendChild(ns);
            List<String> cacheList = zk
                    .getChildren(getZkPath(ConfigEntry.CM, namespace, ""), false);
            if (cacheList != null) {
                for (String server : cacheList) {
                    Element node = doc.createElement("node");
                    String[] units = ConfigEntry.checkManagerServerProcess(server);
                    node.setAttribute("address", units[0]);
                    node.setAttribute("port", units[1]);
                    ns.appendChild(node);
                }
            }
        }

        Element pm = doc.createElement("producermanager");
        root.appendChild(pm);
        List<String> pmNamespaces = zk.getChildren(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE,
                false);
        for (String namespace : pmNamespaces) {
            Element ns = doc.createElement("namespace");
            ns.setAttribute("id", namespace);
            pm.appendChild(ns);
            List<String> cacheList = zk
                    .getChildren(getZkPath(ConfigEntry.PM, namespace, ""), false);
            if (cacheList != null) {
                for (String server : cacheList) {
                    Element node = doc.createElement("node");
                    String[] units = ConfigEntry.checkManagerServerProcess(server);
                    node.setAttribute("address", units[0]);
                    node.setAttribute("port", units[1]);
                    ns.appendChild(node);
                }
            }
        }

        TransformerFactory transFactory = TransformerFactory.newInstance();
        transFactory.setAttribute("indent-number", new Integer(4));// 设置缩进长度为4
        Transformer transformer = transFactory.newTransformer();
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");// 设置自动换行
        DOMSource source = new DOMSource(doc);
        transformer.transform(source, new StreamResult(new BufferedWriter(new OutputStreamWriter(
                new FileOutputStream(xmlPath), "UTF-8"))));
    }

    static TripodConfig getTripodConfigFromZk(String zkAddress) throws Exception {
        ZooKeeper zk = ZkWrapper.getInstance(zkAddress);
        List<String> ccNamespaces = zk.getChildren(Constants.DEFAULT_ZK_CACHE_ZNODE, false);
        List<String> cmNamespaces = zk.getChildren(Constants.DEFAULT_ZK_CACHEMANAGERZNODE, false);
        List<String> pmNamespaces = zk.getChildren(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE,
                false);
        TripodConfig config = new TripodConfig();
        for (String namespace : ccNamespaces) {
            List<String> cacheList = zk
                    .getChildren(getZkPath(ConfigEntry.CC, namespace, ""), false);
            if (cacheList != null) {
                for (String server : cacheList) {
                    config.addServer(namespace, ConfigEntry.CC, server);
                }
            }
        }
        for (String namespace : cmNamespaces) {
            List<String> cacheManagerList = zk.getChildren(
                    getZkPath(ConfigEntry.CM, namespace, ""), false);
            if (cacheManagerList != null) {
                for (String server : cacheManagerList) {
                    config.addServer(namespace, ConfigEntry.CM, server);
                }
            }
        }
        for (String namespace : pmNamespaces) {
            List<String> producerManagerList = zk.getChildren(
                    getZkPath(ConfigEntry.PM, namespace, ""), false);
            if (producerManagerList != null) {
                for (String server : producerManagerList) {
                    config.addServer(namespace, ConfigEntry.PM, server);
                }
            }
        }
        return config;
    }

    static void updateZkConfig(TripodConfig newConfig, String zkAddress) throws Exception {
        initTripod2Zk(zkAddress);
        TripodConfig onlineConfig = getTripodConfigFromZk(zkAddress);
        TripodConfig off = onlineConfig.removeAll(newConfig);
        TripodConfig add = newConfig.removeAll(onlineConfig);
        if (off.entryMap.size() > 0) {
            IOStreamUtil.print("Following will be cut off:\n" + off);
        } else {
            IOStreamUtil.print("No old server cut off.");
        }
        if (add.entryMap.size() > 0) {
            IOStreamUtil.print("Following will be add on:\n" + add);
        } else {
            IOStreamUtil.print("No new server add on.");
        }
        IOStreamUtil.makeSureWithPromt("These will apply");
        ZooKeeper zk = ZkWrapper.getInstance(zkAddress);
        int changeStamp = 0; // for the diff of one changes version
        for (Entry<String, ConfigEntry> m : add.entryMap.entrySet()) {
            changeStamp++;
            String namespace = m.getKey();
            ConfigEntry entry = m.getValue();
            String parentPath = getZkPath(ConfigEntry.CC, namespace, "");
            if (zk.exists(parentPath, false) == null) {
                IOStreamUtil.print("Add path.. " + parentPath);
                zk.create(parentPath, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
            }
            for (String server : entry.getCache()) {
                String zkPath = getZkPath(ConfigEntry.CC, namespace, server);
                IOStreamUtil.print("Add path.. " + zkPath);
                if (zk.exists(zkPath, false) == null) {
                    zk.create(zkPath, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
                } else {
                    IOStreamUtil.print("path已存在 " + zkPath);
                }
            }
            if (zk.exists(parentPath, false) != null) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss.SSSZ");
                zk.setData(parentPath,
                        (df.format(new Date()) + parentPath + "Add" + changeStamp).getBytes(), -1);
            }
            parentPath = getZkPath(ConfigEntry.CM, namespace, "");
            if (zk.exists(parentPath, false) == null) {
                IOStreamUtil.print("Add path.. " + parentPath);
                zk.create(parentPath, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
            }
            for (String server : entry.getCachemanager()) {
                String zkPath = getZkPath(ConfigEntry.CM, namespace, server);
                IOStreamUtil.print("Add path.. " + zkPath);
                if (zk.exists(zkPath, false) == null) {
                    zk.create(zkPath, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
                } else {
                    IOStreamUtil.print("path已存在 " + zkPath);
                }
            }
            if (zk.exists(parentPath, false) != null) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss.SSSZ");
                zk.setData(parentPath,
                        (df.format(new Date()) + parentPath + "Add" + changeStamp).getBytes(), -1);
            }
            parentPath = getZkPath(ConfigEntry.PM, namespace, "");
            if (zk.exists(parentPath, false) == null) {
                IOStreamUtil.print("Add path.. " + parentPath);
                zk.create(parentPath, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
            }
            for (String server : entry.getProducermanager()) {
                String zkPath = getZkPath(ConfigEntry.PM, namespace, server);
                IOStreamUtil.print("Add path.. " + zkPath);
                if (zk.exists(zkPath, false) == null) {
                    zk.create(zkPath, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
                } else {
                    IOStreamUtil.print("path已存在 " + zkPath);
                }
            }
            if (zk.exists(parentPath, false) != null) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss.SSSZ");
                zk.setData(parentPath,
                        (df.format(new Date()) + parentPath + "Add" + changeStamp).getBytes(), -1);
            }
        }
        changeStamp = 0;
        for (Entry<String, ConfigEntry> m : off.entryMap.entrySet()) {
            changeStamp++;
            String namespace = m.getKey();
            ConfigEntry entry = m.getValue();
            for (String server : entry.getCache()) {
                String zkPath = getZkPath(ConfigEntry.CC, namespace, server);
                IOStreamUtil.print("Remove path.. " + zkPath);
                zk.delete(zkPath, -1);
            }
            String parentPath = getZkPath(ConfigEntry.CC, namespace, "");
            if (zk.exists(parentPath, false) != null) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss.SSSZ");
                zk.setData(parentPath,
                        (df.format(new Date()) + parentPath + "Off" + changeStamp).getBytes(), -1);
            }
            if (zk.exists(parentPath, false) != null
                    && zk.getChildren(parentPath, false).size() == 0) {
                IOStreamUtil.print("Remove path.. " + parentPath);
                zk.delete(parentPath, -1);
            }
            for (String server : entry.getCachemanager()) {
                String zkPath = getZkPath(ConfigEntry.CM, namespace, server);
                IOStreamUtil.print("Remove path.. " + zkPath);
                zk.delete(zkPath, -1);
            }
            parentPath = getZkPath(ConfigEntry.CM, namespace, "");
            if (zk.exists(parentPath, false) != null) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss.SSSZ");
                zk.setData(parentPath,
                        (df.format(new Date()) + parentPath + "Off" + changeStamp).getBytes(), -1);
            }
            if (zk.exists(parentPath, false) != null
                    && zk.getChildren(parentPath, false).size() == 0) {
                IOStreamUtil.print("Remove path.. " + parentPath);
                zk.delete(parentPath, -1);
            }
            for (String server : entry.getProducermanager()) {
                String zkPath = getZkPath(ConfigEntry.PM, namespace, server);
                IOStreamUtil.print("Remove path.. " + zkPath);
                zk.delete(zkPath, -1);
            }
            parentPath = getZkPath(ConfigEntry.PM, namespace, "");
            if (zk.exists(parentPath, false) != null) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy_MM_dd_HH:mm:ss.SSSZ");
                zk.setData(parentPath,
                        (df.format(new Date()) + parentPath + "Off" + changeStamp).getBytes(), -1);
            }
            if (zk.exists(parentPath, false) != null
                    && zk.getChildren(parentPath, false).size() == 0) {
                IOStreamUtil.print("Remove path.. " + parentPath);
                zk.delete(parentPath, -1);
            }
        }
        IOStreamUtil.print("zk配置已更新");
    }

    static String getZkPath(String serverType, String namespace, String serverProcess) {
        String suffix = "";
        if (namespace != null && namespace.length() != 0) {
            suffix += (Constants.SEPARATOR + namespace);
        }
        if (serverProcess != null && serverProcess.length() != 0) {
            suffix += (Constants.SEPARATOR + serverProcess);
        }
        if (serverType.equals(ConfigEntry.CC)) {
            return Constants.DEFAULT_ZK_CACHE_ZNODE + suffix;
        } else if (serverType.equals(ConfigEntry.CM)) {
            return Constants.DEFAULT_ZK_CACHEMANAGERZNODE + suffix;
        } else if (serverType.equals(ConfigEntry.PM)) {
            return Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE + suffix;
        } else {
            return null;
        }
    }

    static void createZkMutex(String zkAddress) throws Exception {
        ZooKeeper zk = ZkWrapper.getInstance(zkAddress);
        IOStreamUtil.print("Init zk mutex... ");
        try {
            zk.create("/mutex", null, Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        } catch (NodeExistsException e) {
            // TODO Auto-generated catch block
            IOStreamUtil.print("zk mutex exists, someone else is using this tool");
            System.exit(1);
        }
    }

    static void initTripod2Zk(String zkAddress) throws Exception {
        ZooKeeper zk = ZkWrapper.getInstance(zkAddress);
        if (zk.exists(Constants.DEFAULT_PREFIX, false) == null) {
            zk.create(Constants.DEFAULT_PREFIX, null, Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
        }
        if (zk.exists(Constants.DEFAULT_ZK_CACHE_ZNODE, false) == null) {
            zk.create(Constants.DEFAULT_ZK_CACHE_ZNODE, null, Ids.OPEN_ACL_UNSAFE,
                    CreateMode.PERSISTENT);
        }
        if (zk.exists(Constants.DEFAULT_ZK_CACHE_ZNODE, false) == null) {
            zk.create(Constants.DEFAULT_ZK_CACHE_ZNODE, null, Ids.OPEN_ACL_UNSAFE,
                    CreateMode.PERSISTENT);
        }
        if (zk.exists(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE, false) == null) {
            zk.create(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE, null, Ids.OPEN_ACL_UNSAFE,
                    CreateMode.PERSISTENT);
        }
    }

}
