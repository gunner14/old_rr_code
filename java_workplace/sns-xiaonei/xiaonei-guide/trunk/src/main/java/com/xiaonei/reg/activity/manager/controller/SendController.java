package com.xiaonei.reg.activity.manager.controller;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

@LoginRequired
@Path("sendaward")
public class SendController {
	@Get
	@Post
	public String index(Invocation inv){
		
		return null;
	}
}
