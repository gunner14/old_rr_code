package com.renren.xoa.demo.controllers.jsonparam;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

import com.renren.xoa.commons.annotation.JsonParam;
import com.renren.xoa.commons.test.Blog;

public class JsonParamController {
	
	public JsonParamController() {

	}

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv, @JsonParam @Param("myBlog")  Blog blog) {
		return "@" + blog.toString();
	}
	
}
