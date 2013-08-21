package com.xiaonei.reg.guide.model;

import java.io.Serializable;

public class UserPersonalInfoModel implements Serializable {
    private static final long serialVersionUID = -2333704838481023000L;

    private int id;
    private String inviteCode;
    private String link;
    private int linkStatus;
    private int spaceAll;
    private long lastIp;
    private int source;

    public UserPersonalInfoModel() {
        super();
    }

    public UserPersonalInfoModel(int id, String inviteCode, int lastIp, String link, int linkStatus, int source, int spaceAll) {
        super();
        this.id = id;
        this.inviteCode = inviteCode;
        this.lastIp = lastIp;
        this.link = link;
        this.linkStatus = linkStatus;
        this.source = source;
        this.spaceAll = spaceAll;
    }

    @Override
    public String toString() {
        String restr = "";
        restr += " id: " + id;
        restr += " inviteCode: " + inviteCode;
        restr += " lastIp: " + lastIp;
        restr += " link: " + link;
        restr += " linkStatus: " + linkStatus;
        restr += " source: " + source;
        restr += " spaceAll: " + spaceAll;
        return restr;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getInviteCode() {
        return inviteCode;
    }

    public void setInviteCode(String inviteCode) {
        this.inviteCode = inviteCode;
    }

    public String getLink() {
        return link;
    }

    public void setLink(String link) {
        this.link = link;
    }

    public int getLinkStatus() {
        return linkStatus;
    }

    public void setLinkStatus(int linkStatus) {
        this.linkStatus = linkStatus;
    }

    public int getSpaceAll() {
        return spaceAll;
    }

    public void setSpaceAll(int spaceAll) {
        this.spaceAll = spaceAll;
    }

    public long getLastIp() {
        return lastIp;
    }

    public void setLastIp(long lastIp) {
        this.lastIp = lastIp;
    }

    public int getSource() {
        return source;
    }

    public void setSource(int source) {
        this.source = source;
    }

}
