package com.xiaonei.platform.core.opt.base.chain.impl;

import java.util.HashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 基于web的一个Context的实现。 主要封装HttpServletRequest对象和HttpServletResponse对象。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午10:09:39
 */
@SuppressWarnings("unchecked")
public class WebContextBase extends HashMap implements WebContext {

    private static final long serialVersionUID = 382324069231775315L;

    private transient HttpServletRequest request;

    private transient HttpServletResponse response;

    public WebContextBase() {
    	super(4);	//考虑到WebContextBase的map性质被利用的几率比较小，所以初始化一个较小的容量
    }

    public WebContextBase(HttpServletRequest request, HttpServletResponse response) {
        this.setRequest(request);
        this.setResponse(response);
    }

    public HttpServletRequest getRequest() {
        return request;
    }

    public void setRequest(HttpServletRequest request) {
        this.request = request;
    }

    public HttpServletResponse getResponse() {
        return response;
    }

    public void setResponse(HttpServletResponse response) {
        this.response = response;
    }
}
