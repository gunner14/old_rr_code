/**
 *
 */
package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

/**
 * RegAppAudit.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-8 下午06:25:48
 */
public class RegAppAudit implements Serializable {
    private static final long serialVersionUID = 3799655789340994603L;

    public static final int APP_AUDIT_NEW = 0;
    public static final int APP_AUDIT_OK = 1;

    private int id;
    private int from_id;
    private String from_name;
    private int app_id;
    private String invite_type;
    private String app_token;
    private Date bagin_time;
    private Date end_time;
    private int status;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getFrom_id() {
        return from_id;
    }

    public void setFrom_id(int from_id) {
        this.from_id = from_id;
    }

    public String getFrom_name() {
        return from_name;
    }

    public void setFrom_name(String from_name) {
        this.from_name = from_name;
    }

    public int getApp_id() {
        return app_id;
    }

    public void setApp_id(int app_id) {
        this.app_id = app_id;
    }

    public String getInvite_type() {
        return invite_type;
    }

    public void setInvite_type(String invite_type) {
        this.invite_type = invite_type;
    }

    public String getApp_token() {
        return app_token;
    }

    public void setApp_token(String app_token) {
        this.app_token = app_token;
    }

    public Date getBagin_time() {
        return bagin_time;
    }

    public void setBagin_time(Date bagin_time) {
        this.bagin_time = bagin_time;
    }

    public Date getEnd_time() {
        return end_time;
    }

    public void setEnd_time(Date end_time) {
        this.end_time = end_time;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }
}
