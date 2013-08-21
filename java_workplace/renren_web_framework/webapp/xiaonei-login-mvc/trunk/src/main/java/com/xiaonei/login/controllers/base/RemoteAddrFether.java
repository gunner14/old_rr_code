/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.login.controllers.base;

import javax.servlet.http.HttpServletRequest;

/**
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public interface RemoteAddrFether {

    /**
     * 返回用户实际的远程地址(即IP地址)
     * 
     * @param request
     * @return
     */
    public String getRemoteAddr(HttpServletRequest request);
}
