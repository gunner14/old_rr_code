package com.xiaonei.page.xoa.domain;

/**
 * @author happyshelocks@gmail.com
 * 
 */
public class PageHeadUrls {

    private String headUrl;

    private String largeUrl;

    private String mainUrl;

    private String tinyUrl;
    
    public PageHeadUrls(){
    }

    public PageHeadUrls(String headUrl, String largeUrl, String mainUrl, String tinyUrl) {
        super();
        this.headUrl = headUrl;
        this.largeUrl = largeUrl;
        this.mainUrl = mainUrl;
        this.tinyUrl = tinyUrl;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        this.mainUrl = mainUrl;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

}
