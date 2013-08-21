package com.dodoyo.invite.core.entity;

import java.io.Serializable;
import java.util.Date;

public class InviteUrlParas implements Serializable {

    /**
     * 邀请连接参数表
     */
    private static final long serialVersionUID = 7110690396248775032L;

    private String uuid;

    private int user_id;

    private String email_from;

    private String email_to;

    private String ss;

    private int app_id;

    private String app_token;

    private String md5;

    private String extar_paras;

    private Date time;

//    private int id;

    private String buddy_group;

    private String code;

    private String rt;

    public String getBuddy_group() {
        return buddy_group;
    }

    public void setBuddy_group(String buddy_group) {
        this.buddy_group = buddy_group;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

//    public int getId() {
//        return id;
//    }
//
//    public void setId(int id) {
//        this.id = id;
//    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public int getUser_id() {
        return user_id;
    }

    public void setUser_id(int user_id) {
        this.user_id = user_id;
    }

    public String getEmail_from() {
        return email_from;
    }

    public void setEmail_from(String email_from) {
        this.email_from = email_from;
    }

    public String getEmail_to() {
        return email_to;
    }

    public void setEmail_to(String email_to) {
        this.email_to = email_to;
    }

    public String getSs() {
        return ss;
    }

    public void setSs(String ss) {
        this.ss = ss;
    }

    public int getApp_id() {
        return app_id;
    }

    public void setApp_id(int app_id) {
        this.app_id = app_id;
    }

    public String getApp_token() {
        return app_token;
    }

    public void setApp_token(String app_token) {
        this.app_token = app_token;
    }

    public String getMd5() {
        return md5;
    }

    public void setMd5(String md5) {
        this.md5 = md5;
    }

    public String getExtar_paras() {
        return extar_paras;
    }

    public void setExtar_paras(String extar_paras) {
        this.extar_paras = extar_paras;
    }

    public String getRt() {
        return rt;
    }

    public void setRt(String rt) {
        this.rt = rt;
    }
}
