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
 * 2010-8-25 为了兼容老的 /regimportmsn.do
 * 2010-8-30 为了兼容老的 /regimportmsnbl.do
 * 2010-8-31 为了兼容老的 /import-msn-ns.do
 * 2010-9-1 为了兼容老的 /im-090729.do /import-msn-ns.do /regimportmsndx.do
 */
@LoginRequired
@Path({"/regimportmsn.do","/regimportmsnbl.do","/import-msn-ns.do","/im-090729.do","/import-msn-ns.do","/regimportmsndx.do"})
public class ShowImportMsnController {
    
    @Get
    @Post
    public String index(Invocation inv){
        return "/views/guide/oldactions/regimportmsn.jsp";
    }
}
