package com.xiaonei.reg.usertrace.model;

import java.io.Serializable;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * 2008-10-24
 *
 * @author wei.cheng@opi-corp.com
 */
public class InviteUrlParams implements Serializable {

    private static final long serialVersionUID = 1815820157533583135L;

    private String uuid;
    private int userid;
    private String email_from;
    private String email_to;
    private String ss;
    private int app_id;
    private String app_token;

    private Map<String, String> paras;

    public InviteUrlParams() {
        super();
        paras = new ConcurrentHashMap<String, String>();
    }

    public Map<String, String> getParas() {
        paras.put("uuid", uuid);
        if (0 != userid) {
            paras.put("userid", "" + userid);
        } else {
            paras.put("userid", "");
        }
        paras.put("email_from", email_from);
        paras.put("email_to", email_to);
        paras.put("ss", ss);
        if (0 != app_id) {
            paras.put("app_id", "" + app_id);
        } else {
            paras.put("app_id", "");
        }
        paras.put("app_token", app_token);
        return paras;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public void setUserid(int userid) {
        this.userid = userid;
    }

    public void setEmail_from(String email_from) {
        this.email_from = email_from;
    }

    public void setEmail_to(String email_to) {
        this.email_to = email_to;
    }

    public void setSs(String ss) {
        this.ss = ss;
    }

    public void setApp_id(int app_id) {
        this.app_id = app_id;
    }

    public void setApp_token(String app_token) {
        this.app_token = app_token;
    }

}
