package com.xiaonei.page.xoa.domain;

import com.xiaonei.page.domain.Page;
import com.xiaonei.platform.core.model.User;


public class PageHostPair {

    private Page page;
    private User host;
    private String msg;
    
    public PageHostPair(Page page, User host, String msg) {
        super();
        this.page = page;
        this.host = host;
        this.msg = msg;
    }

    
    public Page getPage() {
        return page;
    }

    
    public void setPage(Page page) {
        this.page = page;
    }

    
    public User getHost() {
        return host;
    }

    
    public void setHost(User host) {
        this.host = host;
    }

    
    public String getMsg() {
        return msg;
    }

    
    public void setMsg(String msg) {
        this.msg = msg;
    }
    
    
}
