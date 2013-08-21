package com.xiaonei.page.xoa.domain;


public class PageShare {
    private String link;
    private int type;
    private String title;
    private String content;
    private String clientIp;
    private String fromName;
    private String fromUniv;
    
    public PageShare(){
        
    }
    public PageShare(String link, int type, String title, String content,String clientIp,String fromName,String fromUniv) {
        this.link = link;
        this.type = type;
        this.title = title;
        this.content = content;
        this.clientIp=clientIp;
        this.fromName=fromName;
        this.fromUniv=fromUniv;
    }

    public String getLink() {
        return link;
    }
    
    public void setLink(String link) {
        this.link = link;
    }
    
    public int getType() {
        return type;
    }
    
    public void setType(int type) {
        this.type = type;
    }
    
    public String getTitle() {
        return title;
    }
    
    public void setTitle(String title) {
        this.title = title;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }

    
    public String getClientIp() {
        return clientIp;
    }

    
    public void setClientIp(String clientIp) {
        this.clientIp = clientIp;
    }

    
    public String getFromName() {
        return fromName;
    }

    
    public void setFromName(String fromName) {
        this.fromName = fromName;
    }

    
    public String getFromUniv() {
        return fromUniv;
    }

    
    public void setFromUniv(String fromUniv) {
        this.fromUniv = fromUniv;
    }
    
    
    
    
}
