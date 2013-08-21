/**
 *
 */
package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

/**
 * PendingUser.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-12 下午08:43:10
 */
public class PendingUser implements Serializable {

    private static final long serialVersionUID = 3372974739005642877L;
    public static final int FLAG_DEFAULT = 1;
    public static final int FLAG_APPLY = 0;
    private int id;
    private Date time;
    private int user;
    private int stage;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public PendingUser(int usr) {
        super();
        this.user = usr;
        this.time = new Date();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public int getUser() {
        return user;
    }

    public void setUser(int user) {
        this.user = user;
    }
}
