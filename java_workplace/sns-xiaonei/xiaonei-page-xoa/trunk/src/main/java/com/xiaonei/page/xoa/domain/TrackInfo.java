package com.xiaonei.page.xoa.domain;

public class TrackInfo {

    private String stateID;

    private String level;

    public TrackInfo(){
        
    }
    public TrackInfo(String stateID, String level) {
        this.stateID = stateID;
        this.level = level;
    }

    public String getStateID() {
        return stateID;
    }

    public void setStateID(String stateID) {
        this.stateID = stateID;
    }

    public String getLevel() {
        return level;
    }

    public void setLevel(String level) {
        this.level = level;
    }

}
