package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

public class InviteInfo implements Serializable {

	private static final long serialVersionUID = -5399085467618445327L;

	private int id;

    private int owner_id;

    private String owner_name;

    private String tinyurl;

    private String from_account;

    private String account;

    private int flag;

    private String nickname;

    private int accountid;

    private int actiontype;

    private String headurl;

    private String accountname;

    private int app_id;

    private Date invite_time;

    public Date getInvite_time() {
        return invite_time;
    }

    public void setInvite_time(Date invite_time) {
        this.invite_time = invite_time;
    }

    public static long getSerialVersionUID() {
        return serialVersionUID;
    }

    public String getAccount() {
        return account;
    }

    public void setAccount(String account) {
        this.account = account;
    }

    public int getAccountid() {
        return accountid;
    }

    public void setAccountid(int accountid) {
        this.accountid = accountid;
    }

    public String getAccountname() {
        return accountname;
    }

    public void setAccountname(String accountname) {
        this.accountname = accountname;
    }

    public int getActiontype() {
        return actiontype;
    }

    public void setActiontype(int actiontype) {
        this.actiontype = actiontype;
    }

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    public String getFrom_account() {
        return from_account;
    }

    public void setFrom_account(String from_account) {
        this.from_account = from_account;
    }

    public String getHeadurl() {
        return headurl;
    }

    public void setHeadurl(String headurl) {
        this.headurl = headurl;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public int getOwner_id() {
        return owner_id;
    }

    public void setOwner_id(int owner_id) {
        this.owner_id = owner_id;
    }

    public String getOwner_name() {
        return owner_name;
    }

    public void setOwner_name(String owner_name) {
        this.owner_name = owner_name;
    }

    public String getTinyurl() {
        return tinyurl;
    }

    public void setTinyurl(String tinyurl) {
        this.tinyurl = tinyurl;
    }

    public void setApp_id(int app_id) {
        this.app_id = app_id;
    }

    public int getApp_id() {
        return app_id;
    }

}
