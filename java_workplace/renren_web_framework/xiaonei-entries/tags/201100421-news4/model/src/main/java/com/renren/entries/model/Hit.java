package com.renren.entries.model;

/**
 * {@link Hit} 表示从系统中检索到的一个条目及其对应的cursor
 * 
 * @author yanghe.liang@renren-inc.com
 * @author qieqie.wang@gmail.com
 * 
 */
public class Hit implements Comparable<Hit> {

    /** 条目ID */
    private long entryId;

    /** 该条目的lower cursor */
    private long cursor;

    public Hit() {
    }

    public Hit(long entryId, long cursor) {
        setEntryId(entryId);
        setCursor(cursor);
    }

    public void setCursor(long cursor) {
        this.cursor = cursor;
    }

    public long getCursor() {
        return cursor;
    }

    public void setEntryId(long entryId) {
        this.entryId = entryId;
    }

    public long getEntryId() {
        return entryId;
    }

    @Override
    public int compareTo(Hit o) {
        if (this.cursor < o.cursor) {
            return 1;
        }
        if (this.cursor > o.cursor) {
            return -1;
        }
        return 0;
    }
}
