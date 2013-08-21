package com.renren.xoa.demo.controllers;


import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class ErrorController {

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv) {
		return new XoaBizErrorBean("XOA逻辑错误");
	}
}
