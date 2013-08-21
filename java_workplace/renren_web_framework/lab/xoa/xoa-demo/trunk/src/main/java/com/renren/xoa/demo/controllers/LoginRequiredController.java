package com.renren.xoa.demo.controllers;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

@LoginRequired
public class LoginRequiredController {
	
	public LoginRequiredController() {

	}

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv) {
		return null;
	}
}
