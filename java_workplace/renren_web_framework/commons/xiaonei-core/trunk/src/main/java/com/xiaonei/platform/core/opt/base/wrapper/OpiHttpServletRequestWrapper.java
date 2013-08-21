/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.platform.core.opt.base.wrapper;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

/**
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
class OpiHttpServletRequestWrapper extends HttpServletRequestWrapper {

    // 用于正确获取请求IP，特别是在代理转发的情况下
    private RemoteAddrFether remoteAddrFether = new RemoteAddrFetherImpl();

    public OpiHttpServletRequestWrapper(HttpServletRequest request) {
        super(request);
    }

    public void setRemoteAddrFether(RemoteAddrFether remoteAddrFether) {
        this.remoteAddrFether = remoteAddrFether;
    }

    @Override
    public String getRemoteAddr() {
        return remoteAddrFether.getRemoteAddr((HttpServletRequest) getRequest());
    }

}
