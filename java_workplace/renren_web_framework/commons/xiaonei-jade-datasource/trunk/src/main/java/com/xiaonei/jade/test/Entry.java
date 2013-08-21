package com.xiaonei.jade.test;

public class Entry {

    private long entryId;

    private String value;

    public long getEntryId() {
        return entryId;
    }

    public void setEntryId(long entryId) {
        this.entryId = entryId;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return "id=" + entryId + "&" + value;
    }
}
