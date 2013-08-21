package com.xiaonei.reg.guide.view;

import java.io.Serializable;

public class RegNotifyView implements Serializable {

    private static final long serialVersionUID = 1136699069965588718L;

    private int id;
    private String name;

    private int inviterId;
    private String inviterName;

    private long ip_long;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

    public String getInviterName() {
        return inviterName;
    }

    public void setInviterName(String inviterName) {
        this.inviterName = inviterName;
    }

    public long getIp_long() {
        return ip_long;
    }

    public void setIp_long(String ip) {
        String[] ipn = ip.split("\\.");
        long ipl = 0;
        ipl += Long.parseLong(ipn[0]) * Math.pow(256, 3);
        ipl += Long.parseLong(ipn[1]) * Math.pow(256, 2);
        ipl += Long.parseLong(ipn[2]) * Math.pow(256, 1);
        ipl += Long.parseLong(ipn[3]) * Math.pow(256, 0);
        this.ip_long = ipl;
    }

}
