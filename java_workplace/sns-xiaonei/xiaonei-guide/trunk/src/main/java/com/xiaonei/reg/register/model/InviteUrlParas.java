/**
 *
 */
package com.xiaonei.reg.register.model;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

import java.io.Serializable;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * InviteUrlParas.java
 *
 * @author tai.wang@opi-corp.com (2008-10-24 - 下午04:36:53)
 */
public class InviteUrlParas implements Serializable {

    private static final long serialVersionUID = 1815820157533583135L;

    private String uuid;
    private int user_id;
    private String email_from;
    private String email_to;
    private int app_id;
    private String app_token;
    private String code;
    private String buddy_group;
    private String ss;

    private Map<String, String> paras;

    public InviteUrlParas() {
        super();
        paras = new ConcurrentHashMap<String, String>();
    }

    public Map<String, String> getParas() {
        if (null != uuid) {
            paras.put("uuid", uuid);
        }
        if (0 != user_id) {
            paras.put("i", "" + user_id);
        }
        if (null != email_from) {
            paras.put("email_from", email_from);
        }
        if (null != email_to) {
            paras.put("e", email_to);
        }
        if (0 != app_id) {
            paras.put("appId", "" + app_id);
        }
        if (null != app_token) {
            paras.put("appToken", app_token);
        }
        if (null != code) {
            paras.put("code", code);
        }
        if (null != buddy_group) {
            paras.put("groupName", buddy_group);
        }
        if (null != ss) {
            paras.put("ss", ss);
        }
        return paras;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public void setUser_id(int user_id) {
        this.user_id = user_id;
    }

    public void setEmail_from(String email_from) {
        this.email_from = email_from;
    }

    public void setEmail_to(String email_to) {
        this.email_to = email_to;
    }

    public void setApp_id(int app_id) {
        this.app_id = app_id;
    }

    public void setApp_token(String app_token) {
        this.app_token = app_token;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public void setBuddy_group(String buddy_group) {
        this.buddy_group = buddy_group;
    }

    public void setSs(String ss) {
        this.ss = ss;
    }

    public void setParas(Map<String, String> paras) {
        this.paras = paras;
    }

    /**
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
                .appendSuper(super.toString()).append("buddy_group",
                        this.buddy_group).append("app_token", this.app_token)
                .append("email_to", this.email_to).append("paras", this.paras)
                .append("ss", this.ss).append("app_id", this.app_id).append(
                        "user_id", this.user_id).append("code", this.code)
                .append("uuid", this.uuid)
                .append("email_from", this.email_from).toString();
    }

}
