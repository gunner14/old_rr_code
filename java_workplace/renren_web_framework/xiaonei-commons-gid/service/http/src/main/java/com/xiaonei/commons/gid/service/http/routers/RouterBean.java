package com.xiaonei.commons.gid.service.http.routers;

import javax.servlet.http.HttpServletRequest;

public class RouterBean implements Router {

    private String target;

    public RouterBean() {
    }

    public RouterBean(String target) {
        setTarget(target);
    }

    public void setTarget(String target) {
        this.target = target;
    }

    @Override
    public String getTarget(long gid, HttpServletRequest request) {
        return target;
    }

}
