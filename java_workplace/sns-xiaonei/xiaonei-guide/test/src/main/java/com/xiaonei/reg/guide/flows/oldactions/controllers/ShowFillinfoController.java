package com.xiaonei.reg.guide.flows.oldactions.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

/**
 * ShowImportMsnController.java
 * @author inshion(xin.yin@opi-corp.com)
 *
 * 2010-8-31 为了兼容老的 fillinfo...
 * 2010-9-1 为了兼容老的/fillinfo-sf.do
 */
@LoginRequired
@Path({"/fillinfojx.do","/fillinfobl.do","/fillinfodx.do","/fill-info-ns.do","/fillinfohs.do","/fi-090729.do","/fillinfo-sf.do"})
public class ShowFillinfoController {
    
    @Get
    @Post
    public String index(Invocation inv){
        return "/views/guide/oldactions/fillinfo.jsp";
    }
}
