package com.xiaonei.commons.gid.service.http.routers;

import javax.servlet.http.HttpServletRequest;

public interface Router {

    public String getTarget(long gid, HttpServletRequest request);
}
