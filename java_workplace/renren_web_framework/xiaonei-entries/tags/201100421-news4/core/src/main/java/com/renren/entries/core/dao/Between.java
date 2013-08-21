package com.renren.entries.core.dao;

/**
 * 
 * @author qieqie.wang@gmai.com
 * 
 */
public class Between {

    public Between() {
    }

    public Between(Object start, Object end) {

        this.start = start;
        this.end = end;
    }

    private Object start;

    private Object end;

    public Object getEnd() {
        return end;
    }

    public Object getStart() {
        return start;
    }

}
