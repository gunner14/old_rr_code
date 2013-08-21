package com.renren.entrybase.mysql.model;

import java.util.ArrayList;
import java.util.List;

import com.renren.entrybase.model.Entry;

/**
 * {@link EntryPage} 代表一些条目的集合
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * 
 */
public class EntryPage {

    /** 本页查找的结果集内容 */
    private List<Entry> hits;

    /** 结果集内容的entry视图，这个字段在调用 {@link #getEntries()} 时才被赋值 */
    private transient long[] entries;

    /** 结果集内容的entry视图，这个字段在调用 {@link #getEntryList()} 时才被赋值 */
    private transient List<Long> entryList;

    /**
     * 构造一个尚未设置任何值的结果集
     */
    public EntryPage() {
    }

    /**
     * 创建一个完全可用的结果集
     * 
     * @param cursor
     * @param hits
     */
    public EntryPage(List<Entry> hits) {
        this.hits = hits;
    }

    /**
     * 设置结果集
     * 
     * @param hits
     */
    public void setHits(List<Entry> hits) {
        this.hits = hits;
        this.entries = null;
        this.entryList = null;
    }

    /**
     * 结果集
     * 
     * @return
     */
    public List<Entry> getHits() {
        return hits;
    }

    public Entry getFirstEntry() {
        return hits.get(0);
    }

    public Entry getLastEntry() {
        return hits.get(hits.size() - 1);
    }

    /**
     * 返回本页结果集的个数，如果没有任何结果集返回0
     * 
     * @return
     */
    public int size() {
        return hits == null ? 0 : hits.size();
    }

    /**
     * 返回某个结果
     * 
     * @param index 从0开始
     * @return
     */
    public Entry getHit(int index) {
        if (hits == null) {
            throw new NullPointerException("hits is null");
        }
        return hits.get(index);
    }

    /**
     * 结果集，返回的是条目的ID数组
     * 
     * @see #getEntryList()
     * 
     * @return
     */

    public long[] getEntries() {
        if (hits == null) {
            return null;
        }
        if (entries == null) {
            long[] entries = new long[hits.size()];
            for (int i = 0; i < entries.length; i++) {
                entries[i] = hits.get(i).getId();
            }
            this.entries = entries;
        }
        return entries;
    }

    /**
     * 结果集，返回的是条目的ID数组
     * 
     * @see #getEntries()
     * @return
     */
    public List<Long> getEntryList() {
        if (hits == null) {
            return null;
        }
        if (entryList == null) {
            List<Long> entries = new ArrayList<Long>(hits.size());
            for (int i = 0; i < hits.size(); i++) {
                entries.add(hits.get(i).getId());
            }
            this.entryList = entries;
        }
        return entryList;
    }

    /**
     * 在该结果集中增加一些游标同向、更大或更小的结果
     * 
     * @param appendix
     */
    public void appendHits(List<Entry> appendix) {
        if (appendix == null || appendix.size() == 0) {
            return;
        }
        if (this.hits == null || hits.size() == 0) {
            this.setHits(appendix);
        } else {
            this.hits.addAll(appendix);
            setHits(this.hits);
        }
    }

}
