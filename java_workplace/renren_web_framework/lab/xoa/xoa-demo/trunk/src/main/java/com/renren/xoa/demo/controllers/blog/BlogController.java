package com.renren.xoa.demo.controllers.blog;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;

import com.renren.xoa.commons.test.Blog;

public class BlogController {
	@Get
	public Object index(Invocation inv) {
		Blog blog = new Blog();
		return blog;
	}
}