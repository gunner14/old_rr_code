package com.renren.sns.guide.model;


public class GuideKaixinHotStarModel {
    private int id;
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getNetwork() {
        return network;
    }
    
    public void setNetwork(String network) {
        this.network = network;
    }
    
    public String getTinyFullUrl() {
        return tinyFullUrl;
    }
    
    public void setTinyFullUrl(String tinyFullUrl) {
        this.tinyFullUrl = tinyFullUrl;
    }
    private String name;
    private String network;
    private String tinyFullUrl;
    public void setId(int id) {
        this.id = id;
    }
    public int getId() {
        return id;
    }
}
