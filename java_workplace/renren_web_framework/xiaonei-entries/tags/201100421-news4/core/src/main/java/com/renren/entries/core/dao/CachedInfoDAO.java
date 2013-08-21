package com.renren.entries.core.dao;

import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.model.Entry;
import com.renren.xcache.XCache;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class CachedInfoDAO {

    private InfoDAO infoDAO;

    private XCache<String> infoCache;

    private int expireInSecond;

    public CachedInfoDAO() {
    }

    public CachedInfoDAO(InfoDAO infoDAO) {
        setInfoDAO(infoDAO);
    }

    public InfoDAO getInfoDAO() {
        return infoDAO;
    }

    @Autowired
    public void setInfoDAO(InfoDAO infoDAO) {
        this.infoDAO = infoDAO;
    }

    public XCache<String> getInfoCache() {
        return infoCache;
    }

    public void setInfoCache(XCache<String> infoCache) {
        this.infoCache = infoCache;
    }

    public int getExpireInSecond() {
        return expireInSecond;
    }

    public void setExpireInSecond(int expireInSecond) {
        this.expireInSecond = expireInSecond;
    }

    public void delete(long entryId) {
        infoDAO.delete(entryId);
        infoCache.delete(infoKey(entryId));
    }

    public boolean deleteCache(long entryId) {
        return infoCache.delete(infoKey(entryId));
    }
    
    public Entry get(long entryId) {
        String info = infoCache.get(infoKey(entryId));
        if (info != null) {
            return toEntry(entryId, info);
        }
        Entry entry = infoDAO.get(entryId);
        if (entry == null) {
            infoCache.set(infoKey(entryId), "<null>", expireInSecond);
        } else {
            infoCache.set(infoKey(entryId), entry.getInfoString(), expireInSecond);
        }
        return entry;
    }

    public Entry[] gets(long[] entries) {
        String[] keys = new String[entries.length];
        for (int i = 0; i < keys.length; i++) {
            keys[i] = "info_" + entries[i];
        }
        Entry[] array = new Entry[entries.length];
        Map<String, String> map = infoCache.getMulti(keys);
        for (int i = 0; i < array.length; i++) {
            String info = map.get(keys[i]);
            if (info != null) {
                array[i] = toEntry(entries[i], info);
            } else {
                Entry entry = infoDAO.get(entries[i]);
                if (entry == null) {
                    infoCache.set(infoKey(entries[i]), "<null>", expireInSecond);
                } else {
                    infoCache.set(infoKey(entries[i]), entry.getInfoString(), expireInSecond);
                }
                array[i] = entry;
            }
        }
        //        String[] infos = infoCache.getMultiAsArray(keys);
        //        for (int i = 0; i < infos.length; i++) {
        //            String info = infos[i];
        //            if (info != null) {
        //                array[i] = toEntry(entries[i], info);
        //            } else {
        //                array[i] = get(entries[i]);
        //            }
        //        }
        return array;
    }

    public void save(long entryId, String value) {
        infoDAO.save(entryId, value);
        infoCache.set(infoKey(entryId), value, expireInSecond);
    }
   
    /*public boolean cas(long entryId,  String originValue, String newValue) {
        boolean ret = infoDAO.cas(entryId, originValue, newValue);
        if (ret) { //cas更新成功了
            infoCache.set(infoKey(entryId), newValue, expireInSecond);
        }
        return ret;
    }*/
    
    private String infoKey(long entryId) {
        return String.valueOf(entryId);
    }

    private Entry toEntry(long entryId, String info) {
        if (info.equals("<null>")) {
            return null;
        }
        return new Entry(entryId, Entry.toFields(info));
    }

}
