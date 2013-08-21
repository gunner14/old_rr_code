package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

public class NullController {
	
	public NullController() {

	}

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv) {
		return null;
	}
}
