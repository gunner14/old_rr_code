package com.xiaonei.click.business.persistence;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

import com.xiaonei.click.BaseTestClass;

public class MemoryPersistence extends BaseTestClass implements Persistence {

    private static Persistence _instance = new MemoryPersistence();

    private MemoryPersistence() {
    }

    public static Persistence getInstance() {
        return _instance;
    }

    AtomicInteger key = new AtomicInteger(0);

    Map<Long, Object> storage = new HashMap<Long, Object>();

    private long getStorageKey() {

        return key.getAndIncrement();
    }

    @Override
    public void set(Serializable o) {
        storage.put(getStorageKey(), o);
    }

    public static void init() {
        PersistenceManager.setPersistence(_instance);
    }

    public void restore() {
        storage.clear();
        key.set(0);
    }

    @Override
    public Object get(Object key) {
        return storage.get(Long.parseLong(key.toString()));
    }

    public long size() {
        return storage.size();
    }

}
