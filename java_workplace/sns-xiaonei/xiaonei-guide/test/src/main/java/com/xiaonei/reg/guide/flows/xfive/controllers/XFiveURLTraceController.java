package com.xiaonei.reg.guide.flows.xfive.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path({"xtrs","autr.do"})
@IgnoreUserStatusImportedValidation
public class XFiveURLTraceController {
    
    @Get
    @Post
	public String get(Invocation inv,CacheManager cm){
		return "@OK";
	}
	
	
}
