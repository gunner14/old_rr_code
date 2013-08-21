package com.renren.entries.mysql.model;

/**
 * 
 * @author qieqie.wang@gmai.com
 * 
 */
public class Range<T> {

    public Range() {
    }

    public Range(T start, T end) {
        this.start = start;
        this.end = end;
    }

    private T start;

    private T end;

    public T getEnd() {
        return end;
    }

    public T getStart() {
        return start;
    }

    public void setEnd(T end) {
        this.end = end;
    }

    public void setStart(T start) {
        this.start = start;
    }

    @Override
    public String toString() {
        return start + "-" + end;
    }

}
