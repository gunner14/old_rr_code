package com.xiaonei.click.business.persistence;

import java.io.IOException;
import java.io.Serializable;
import java.net.InetSocketAddress;

import net.spy.memcached.MemcachedClient;

import org.json.JSONArray;
import org.json.JSONException;

import com.xiaonei.click.admin.ReloadAble;
import com.xiaonei.click.configure.Config;
import com.xiaonei.click.configure.Config.Key;
import com.xiaonei.click.constants.Constants;

/**
 * 使用memcached协议的持久化实现
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class MemcachedPersistence implements Persistence, ReloadAble {

    private static String STORAGE_KEY = "storage_index";

    private MemcachedClient mc = null;

    private MemcachedClient keyServer = null;

    public MemcachedPersistence() {
        reConfig();

        if (mc != null && mc.get(STORAGE_KEY) == null) {
            mc.add(STORAGE_KEY, -1, 0);
        }

    }

    @Override
    public void set(Serializable o) {
        mc.set(Long.toString(getStorageKey()), -1, o.toString());
    }

    private long getStorageKey() {
        return keyServer.incr(STORAGE_KEY, 1, 0);

    }

    @Override
    public Object get(Object key) {
        return mc.get(key.toString());

    }

    private void reConfig() {
        JSONArray config = null;
        try {
            config = new JSONArray(Config.getInstance().get(Config.Key.MEMCACHED_SERVER, this)
                    .toString());
        } catch (JSONException e1) {
            throw new RuntimeException("MEMCACHED_SERVER Configure error use prev config!", e1);

        }

        if (config == null || config.length() < 1) {
            throw new RuntimeException("MEMCACHED_SERVER Configure error use prev config!");
        }
        MemcachedClient newMc = null;
        MemcachedClient newKeyServer = null;
        try {
            InetSocketAddress[] addresses = new InetSocketAddress[config.length()];
            for (int i = 0; i < config.length(); i++) {
                String server = config.getJSONObject(i).getString(Constants.MEMCACHED_HOST);
                int port = config.getJSONObject(i).getInt(Constants.MEMCACHED_PORT);
                addresses[i] = new InetSocketAddress(server, port);
                newMc = new MemcachedClient(addresses);
            }
            String server = config.getJSONObject(0).getString(Constants.MEMCACHED_HOST);
            int port = config.getJSONObject(0).getInt(Constants.MEMCACHED_PORT);
            newKeyServer = new MemcachedClient(new InetSocketAddress(server, port));

        } catch (JSONException e1) {
            throw new RuntimeException("MEMCACHED_SERVER Configure error use prev config!", e1);
        } catch (IOException e) {
            throw new RuntimeException("MEMCACHED_SERVER Configure error use prev config!", e);
        }

        mc = newMc;
        keyServer = newKeyServer;
    }

    @Override
    public void reload(Key k) {
        if (k.equals(Key.MEMCACHED_SERVER)) {
            reConfig();
        }
    }

    @Override
    public void reloadAll() {
        // TODO Auto-generated method stub

    }
}
