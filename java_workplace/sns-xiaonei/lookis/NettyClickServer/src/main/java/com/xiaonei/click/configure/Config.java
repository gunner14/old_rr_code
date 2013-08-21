/**
 * 
 */
package com.xiaonei.click.configure;

import java.io.FileInputStream;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import com.xiaonei.click.admin.ReloadAble;

/**
 * 
 * 配置文件管理类
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public final class Config implements ReloadAble {

    public static enum Key {
        PORT, STDOUT, STDERR, CHECK_INTERVAL, DISPATCH_RATE, REQUEST_FILTER, MEMCACHED_SERVER, KEEP_ALIVE;
    }

    //监控配置文件
    private ScheduledExecutorService configMonitor = Executors.newSingleThreadScheduledExecutor();

    private Map<Key, Object> value = new HashMap<Key, Object>();

    private volatile Map<Key, List<WeakReference<ReloadAble>>> registers = new HashMap<Key, List<WeakReference<ReloadAble>>>();

    private static String configureFile = null;

    private static volatile Config _instance = null;

    public static void init(String configureFile) {
        Config.configureFile = configureFile;
    }

    public static Config getInstance() {
        if (_instance == null) {
            synchronized (Config.class) {
                if (_instance == null) {
                    _instance = new Config(Config.configureFile);
                }
            }
        }
        return _instance;
    }

    //初始化时加载第一次的配置
    private Config(String configureFile) {
        if (configureFile == null) throw new NullPointerException(
                "Please init the Config with init(String configureFile)");
        Properties p = new Properties();
        FileInputStream is = null;
        try {
            is = new FileInputStream(configureFile);
            p.load(is);
        } catch (Exception e) {
            throw new RuntimeException("Load configure failure");
        } finally {
            try {
                if (is != null) {
                    is.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        for (Key k : Key.values()) {
            if (p.get(k.toString()) == null) {
                throw new RuntimeException("Can't running the server without the configure:"
                        + k.toString());
            } else {
                value.put(k, p.get(k.toString()));
            }
        }
        startConfigMonitor(this);
    }

    //开启监听线程，检查配置文件变化
    private void startConfigMonitor(final Config config) {
        configMonitor.scheduleAtFixedRate(new Runnable() {

            @Override
            public void run() {
                try {
                    config.reloadAll();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 60, 10, TimeUnit.SECONDS);
    }

    //最常用的方法，获取一个参数，并且如果愿意，就注册成为一个监听者，监控参数变化
    public Object get(Key k, ReloadAble reload) {
        if (reload != null) register(k, reload);
        return value.get(k);
    }

    private void register(Key k, ReloadAble reload) {
        if (registers.get(k) == null) {
            synchronized (registers) {
                if (registers.get(k) == null) {
                    registers.put(k, new ArrayList<WeakReference<ReloadAble>>());
                }
            }
        }
        registers.get(k).add(new WeakReference<ReloadAble>(reload));
    }

    //重新加载配置
    @Override
    public void reloadAll() {
        Map<Key, Object> newConfigure = new HashMap<Key, Object>();
        FileInputStream is = null;
        try {
            Properties p = new Properties();
            is = new FileInputStream(configureFile);
            p.load(is);

            for (Key k : Key.values()) {
                if (p.get(k.toString()) == null) throw new RuntimeException(
                        "Can't running the server without the configure:" + k.toString());
                newConfigure.put(k, p.get(k.toString()));
            }
        } catch (Exception e) {
            //如果配置加载失败，继续使用老的配置
            System.err.println("Configuration Error,Running with old config.");
            e.printStackTrace();
            return;
        } finally {
            try {
                if (is != null) is.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        if (!value.equals(newConfigure)) {
            //查出具体哪个配置改变了
            List<Key> changedKeys = new ArrayList<Key>();
            for (Entry<Key, Object> entry : value.entrySet()) {
                if (!entry.getValue().equals(newConfigure.get(entry.getKey()))) {
                    changedKeys.add(entry.getKey());
                }
            }
            value = newConfigure;
            for (Key key : changedKeys) {
                notifyReloadAble(key);
            }
        }
    }

    //通知具体对应的改变配置的监听者
    private void notifyReloadAble(final Key k) {
        System.out.println("Configure has changed! reload:" + k.toString());
        ExecutorService es = Executors.newCachedThreadPool();
        if (registers.get(k) == null) return;
        Iterator<WeakReference<ReloadAble>> iter = registers.get(k).iterator();
        while (iter.hasNext()) {
            final ReloadAble r = iter.next().get();
            if (r != null) {
                es.submit(new Runnable() {

                    @Override
                    public void run() {
                        r.reload(k);
                    }
                });
            } else {
                iter.remove();
            }
        }

    }

    //忽略自己的配置
    @Override
    public void reload(Key k) {
        // TODO Auto-generated method stub

    }

}