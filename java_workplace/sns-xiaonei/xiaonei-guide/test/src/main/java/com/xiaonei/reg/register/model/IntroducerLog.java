package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

/**
 * IntroducerLog.java
 *
 * @author drizzt drizzt.wang@gmail.com 2006-10-5 下午03:48:35
 */
public class IntroducerLog implements Serializable {
    private static final long serialVersionUID = -4022715871191565448L;
    private int id;
    private int introducer;
    private int newbie;
    private Date time;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getIntroducer() {
        return introducer;
    }

    public void setIntroducer(int introducer) {
        this.introducer = introducer;
    }

    public int getNewbie() {
        return newbie;
    }

    public void setNewbie(int newbie) {
        this.newbie = newbie;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }
}