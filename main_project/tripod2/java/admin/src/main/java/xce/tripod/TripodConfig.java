package xce.tripod;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.json.JSONArray;
import org.json.JSONObject;

class TripodConfig {

    static final String NS = "namespace";

    /*
     * key为namespace
     * 值为配置条目ConfigEntry
    */
    Map<String, ConfigEntry> entryMap = new HashMap<String, ConfigEntry>();

    public String toString() {
        return entryMap.toString();
    }

    public void addServer(String namespace, String type, String toBeAddServer) throws Exception {
        if (!entryMap.containsKey(namespace)) {
            entryMap.put(namespace, new ConfigEntry(namespace));
        }
        entryMap.get(namespace).addServer(type, toBeAddServer);
    }

    public void cutHalf(String namespace, String type) throws Exception {
        List<String> servers = entryMap.get(namespace).getServers(type);
        if (servers == null || servers.size() == 0) {
            IOStreamUtil.print("no servers in " + namespace + "  " + type);
        }
        Collections.sort(servers);
        List<String> toBeCutHalf = null;
        int halfSize = (servers.size() + 1) / 2;
        IOStreamUtil.print("servers in " + namespace + "  " + type + "\n" + servers);
        IOStreamUtil.print("first half or second half ? (a/b):");
        char choice = IOStreamUtil.getChar();
        switch (choice) {
            case 'a':
                toBeCutHalf = servers.subList(0, halfSize);
                break;
            case 'b':
                toBeCutHalf = servers.subList(halfSize, servers.size());
                break;
            default:
                IOStreamUtil.print("选择无效，程序退出");
                System.exit(1);
        }
        IOStreamUtil.makeSureWithPromt("切下：\n" + toBeCutHalf);
        cutServer(toBeCutHalf);
    }

    public void cutServer(List<String> toBeOffServers) throws Exception {
        for (Entry<String, ConfigEntry> m : entryMap.entrySet()) {
            m.getValue().cutServer(toBeOffServers);
        }
    }

    static TripodConfig praseFromJson(String jsonPath) throws Exception {
        String jsonString = IOStreamUtil.readFile(jsonPath);
        JSONObject jsonConfig = new JSONObject(jsonString);
        TripodConfig config = new TripodConfig();
        Iterator<?> iter = jsonConfig.keys();
        while (iter.hasNext()) {
            String namespace = (String) iter.next();
            JSONObject jsonEntry = (JSONObject) jsonConfig.get(namespace);
            Iterator<?> it = jsonEntry.keys();
            while (it.hasNext()) {
                String serverType = (String) it.next();
                JSONArray jsonServers = jsonEntry.getJSONArray(serverType);
                for (int j = 0; j < jsonServers.length(); j++) {
                    config.addServer(namespace, serverType, jsonServers.getString(j));
                }
            }
        }
        return config;
    }

    void dumpToJson(String dumpPath) throws Exception {
        JSONObject jsonConfig = new JSONObject();
        for (Entry<String, ConfigEntry> m : entryMap.entrySet()) {
            String namespace = m.getKey();
            ConfigEntry entry = m.getValue();
            JSONObject jsonEntry = new JSONObject();
            JSONArray jsonCaches = new JSONArray();
            for (String server : entry.getCache()) {
                jsonCaches.put(server);
            }
            jsonEntry.put(ConfigEntry.CC, jsonCaches);
            JSONArray jsonCacheManagers = new JSONArray();
            for (String server : entry.getCachemanager()) {
                jsonCacheManagers.put(server);
            }
            jsonEntry.put(ConfigEntry.CM, jsonCacheManagers);
            JSONArray jsonProducerManagers = new JSONArray();
            for (String server : entry.getProducermanager()) {
                jsonProducerManagers.put(server);
            }
            jsonEntry.put(ConfigEntry.PM, jsonProducerManagers);
            jsonConfig.put(namespace, jsonEntry);
        }
        IOStreamUtil.writeFile(dumpPath, jsonConfig.toString(4));
    }

    /**
     * @param another config
     * @return 本config中包含，但newConfig中不包含的结构
     */
    public TripodConfig removeAll(TripodConfig newConfig) {
        Set<String> nsIntersection = new HashSet<String>();
        nsIntersection.addAll(entryMap.keySet());
        nsIntersection.retainAll(newConfig.entryMap.keySet());
        Set<String> nsDiff = new HashSet<String>();
        nsDiff.addAll(entryMap.keySet());
        nsDiff.removeAll(newConfig.entryMap.keySet());
        TripodConfig result = new TripodConfig();
        for (String namespace : nsDiff) {
            ConfigEntry entry = entryMap.get(namespace);
            if (entry != null && !entry.isEmpty()) {

                result.entryMap.put(namespace, entry);
            }
        }
        for (String namespace : nsIntersection) {
            ConfigEntry entryDiff = entryMap.get(namespace).removeAll(
                    newConfig.entryMap.get(namespace));
            if (entryDiff != null && !entryDiff.isEmpty()) {
                result.entryMap.put(namespace, entryDiff);
            }
        }
        return result;
    }

}
