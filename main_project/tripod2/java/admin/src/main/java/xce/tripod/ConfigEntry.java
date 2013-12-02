package xce.tripod;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

/*
 * 以namespace为索引的配置条目
*/
public class ConfigEntry {

    static final String BIZ = "biz";

    static final String CC = "cache";

    static final String CM = "cachemanager";

    static final String PM = "producermanager";

    /*
     * key为"biz"，"cache"，"cachemanager"，"producermanager"
     * 值为server列表
    */
    private String namespace = "";

    private List<String> bizs = new LinkedList<String>();

    private List<String> caches = new LinkedList<String>();

    private List<String> cachemanagers = new LinkedList<String>();

    private List<String> producermanagers = new LinkedList<String>();

    public ConfigEntry(String namespace) {
        this.namespace = namespace;
    }

    public String toString() {
        return BIZ + bizs.toString() + "\n" + CC + caches.toString() + "\n" + CM
                + cachemanagers.toString() + "\n" + PM + producermanagers.toString() + "\n";
    }

    public static void checkValidType(String type) throws Exception {
        if (!type.equals(CC) && !type.equals(CM) && !type.equals(PM) && !type.equals(BIZ)) {
            throw new Exception("非法类型 " + type);
        }
    }

    public static String[] checkCacheServerProcess(String server) throws Exception {
        String[] units = server.split(":");
        if (units.length != 3) {
            throw new Exception("非法cache server " + server);
        }
        try {
            Integer.parseInt(units[2]);
        } catch (Exception e) {
            throw new Exception("非法cache server " + server);
        }
        return units;
    }

    public static String[] checkManagerServerProcess(String server) throws Exception {
        String[] units = server.split(":");
        if (units.length != 2) {
            throw new Exception("非法 server " + server);
        }
        try {
            Integer.parseInt(units[1]);
        } catch (Exception e) {
            throw new Exception("非法 server " + server);
        }
        return units;
    }

    public boolean isEmpty() {
        return (caches == null || caches.size() == 0)
                && (cachemanagers == null || cachemanagers.size() == 0)
                && (producermanagers == null || producermanagers.size() == 0);
    }

    public LinkedList<String> getBiz() {
        return new LinkedList<String>(bizs);
    }

    public LinkedList<String> getCache() {
        return new LinkedList<String>(caches);
    }

    public LinkedList<String> getCachemanager() {
        return new LinkedList<String>(cachemanagers);
    }

    public LinkedList<String> getProducermanager() {
        return new LinkedList<String>(producermanagers);
    }

    public LinkedList<String> getServers(String type) {
        if (type.equals(CC)) {
            return getCache();
        } else if (type.equals(CM)) {
            return getCachemanager();
        } else if (type.equals(PM)) {
            return getProducermanager();
        } else if (type.equals(BIZ)) {
            return getBiz();
        } else {
            return null;
        }
    }

    public void addServer(String type, String toBeAddServer) throws Exception {
        checkValidType(type);
        if (type.equals(CC)) {
            checkCacheServerProcess(toBeAddServer);
            if (caches.contains(toBeAddServer)) {
                throw new Exception("已存在server " + toBeAddServer);
            }
            caches.add(toBeAddServer);
            Collections.sort(caches);
        } else if (type.equals(CM)) {
            checkManagerServerProcess(toBeAddServer);
            if (cachemanagers.contains(toBeAddServer)) {
                throw new Exception("已存在server " + toBeAddServer);
            }
            cachemanagers.add(toBeAddServer);
            Collections.sort(cachemanagers);
        } else if (type.equals(PM)) {
            checkManagerServerProcess(toBeAddServer);
            if (producermanagers.contains(toBeAddServer)) {
                throw new Exception("已存在server " + toBeAddServer);
            }
            producermanagers.add(toBeAddServer);
            Collections.sort(producermanagers);
        } else {
            if (bizs.contains(toBeAddServer)) {
                throw new Exception("已存在biz " + toBeAddServer);
            }
            bizs.add(toBeAddServer);
        }
    }

    public void cutServer(List<String> toBeOffServers) throws Exception {
        List<String> toRemoveServers = new LinkedList<String>();
        for (String onlineServer : caches) {
            for (String toBeOffServer : toBeOffServers) {
                if (onlineServer.indexOf(toBeOffServer) >= 0) {
                    toRemoveServers.add(onlineServer);
                    break;
                }
            }
        }
        if (caches.size() == toRemoveServers.size() && toRemoveServers.size() != 0) {
            IOStreamUtil.makeSureWithPromt("完成后namespace下所有cache server都会被切掉!!!: " + namespace
                    + ":\n" + toRemoveServers);
        }
        caches.removeAll(toRemoveServers);
        toRemoveServers = new LinkedList<String>();
        for (String onlineServer : cachemanagers) {
            for (String toBeOffServer : toBeOffServers) {
                if (onlineServer.indexOf(toBeOffServer) >= 0) {
                    toRemoveServers.add(onlineServer);
                    break;
                }
            }
        }
        if (cachemanagers.size() == toRemoveServers.size() && toRemoveServers.size() != 0) {
            IOStreamUtil.makeSureWithPromt("完成后namespace下所有cachemanager server都会被切下线!!!: "
                    + namespace + ":\n" + toRemoveServers);
        }
        cachemanagers.removeAll(toRemoveServers);
        toRemoveServers = new LinkedList<String>();
        for (String onlineServer : producermanagers) {
            for (String toBeOffServer : toBeOffServers) {
                if (onlineServer.indexOf(toBeOffServer) >= 0) {
                    toRemoveServers.add(onlineServer);
                    break;
                }
            }
        }
        if (producermanagers.size() == toRemoveServers.size() && toRemoveServers.size() != 0) {
            IOStreamUtil.makeSureWithPromt("完成后namespace下所有producermanager server都会被切下线!!!: "
                    + namespace + ":\n" + toRemoveServers);
        }
        producermanagers.removeAll(toRemoveServers);
    }

    /**
     * @param another entry
     * @return 本entry中包含，但newEntry中不包含的结构
     */
    public ConfigEntry removeAll(ConfigEntry newEntry) {
        if (!this.namespace.equals(this.namespace)) {
            //namsapce不同不比较
            return null;
        }
        ConfigEntry result = new ConfigEntry(namespace);
        result.caches.addAll(this.caches);
        result.caches.removeAll(newEntry.caches);
        result.cachemanagers.addAll(this.cachemanagers);
        result.cachemanagers.removeAll(newEntry.cachemanagers);
        result.producermanagers.addAll(this.producermanagers);
        result.producermanagers.removeAll(newEntry.producermanagers);
        return result;
    }
}
