package com.renren.entries.model;

import java.util.ArrayList;
import java.util.List;

import org.codehaus.jackson.annotate.JsonIgnore;

/**
 * {@link EntryPage} 代表一些条目的集合
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * 
 */
public class EntryPage {

    /** 该结果集的上限游标 */
    private long cursor;

    /** 本次结果结相对cursor的方向 */
    private boolean forward;

    /** 结果集内容 */
    private List<Hit> hits;

    /** 结果集内容的entry视图，这个字段在调用 {@link #getEntries()} 时才被赋值 */
    private long[] entries;

    /** 结果集内容的entry视图，这个字段在调用 {@link #getEntryList()} 时才被赋值 */
    private List<Long> entryList;

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
    public EntryPage(long cursor, boolean forward, List<Hit> hits) {
        this.cursor = cursor;
        this.forward = forward;
        this.hits = hits;
    }

    public void setCursor(long cursor) {
        this.cursor = cursor;
    }

    public long getCursor() {
        return cursor;
    }

    @JsonIgnore
    long getLastCursor() {
        return hits == null || hits.size() == 0 ? (forward ? 0 : Long.MAX_VALUE) : hits.get(
                hits.size() - 1).getCursor();
    }

    /**
     * 返回本结果集的上限游标
     * 
     * @return
     */
    @JsonIgnore
    public long getUpperCursor() {
        return forward ? getCursor() : getLastCursor();
    }

    /**
     * 返回本结果集的下限游标
     * 
     * @return
     */
    @JsonIgnore
    public long getLowerCursor() {
        return forward ? getLastCursor() : getCursor();
    }

    /**
     * 返回指定结果的游标
     * 
     * @param index
     * @return
     */
    public long getCursorAt(int index) {
        if (hits == null) {
            throw new NullPointerException("hits is null");
        }
        return hits.size() == 0 && index == 0 ? 0 : hits.get(index).getCursor();
    }

    public void setForward(boolean forward) {
        this.forward = forward;
    }

    public boolean isForward() {
        return forward;
    }

    /**
     * 设置结果集
     * 
     * @param hits
     */
    public void setHits(List<Hit> hits) {
        this.hits = hits;
        this.entries = null;
        this.entryList = null;
    }

    /**
     * 结果集
     * 
     * @return
     */
    public List<Hit> getHits() {
        return hits;
    }

    /**
     * 结果集，返回的是条目的ID数组
     * 
     * @see #getEntryList()
     * 
     * @return
     */
    @JsonIgnore
    public long[] getEntries() {
        if (hits == null) {
            return null;
        }
        if (entries == null) {
            long[] entries = new long[hits.size()];
            for (int i = 0; i < entries.length; i++) {
                entries[i] = hits.get(i).getEntryId();
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
    @JsonIgnore
    public List<Long> getEntryList() {
        if (hits == null) {
            return null;
        }
        if (entryList == null) {
            List<Long> entries = new ArrayList<Long>(hits.size());
            for (int i = 0; i < hits.size(); i++) {
                entries.add(hits.get(i).getEntryId());
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
    public void appendHits(List<Hit> appendix) {
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

    /**
     * 截取前几个结果作为一个结果集合返回，返回的结果集可能是本身
     * 
     * @param newLength
     * @return
     */
    public EntryPage truncates(int newLength) {
        if (hits == null || hits.size() <= newLength) {
            return this;
        } else {
            // sublist之后就不能append hits了，不过也不打算还能append
            return new EntryPage(cursor, this.forward, hits.subList(0, newLength));
        }
    }

}
