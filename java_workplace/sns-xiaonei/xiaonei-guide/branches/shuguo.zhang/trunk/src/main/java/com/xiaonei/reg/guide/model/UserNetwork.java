package com.xiaonei.reg.guide.model;

import java.io.Serializable;

public class UserNetwork implements Serializable {

    /**
     *
     */
    private static final long serialVersionUID = 7035159326423352898L;

    private int networkId;
    private String networkName;
    private String year;

    private int stage;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public int getNetworkId() {
        return networkId;
    }

    public void setNetworkId(int networkId) {
        this.networkId = networkId;
    }

    public String getNetworkName() {
        return networkName;
    }

    public void setNetworkName(String networkName) {
        this.networkName = networkName;
    }

    public String getYear() {
        return year;
    }

    public void setYear(String year) {
        this.year = year;
    }


}