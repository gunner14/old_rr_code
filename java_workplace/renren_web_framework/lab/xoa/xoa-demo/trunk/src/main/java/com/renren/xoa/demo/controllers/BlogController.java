package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.test.Blog;
import com.xiaonei.commons.interceptors.access.HostHolder;

public class BlogController {

	@Autowired
	private HostHolder hostHolder;
	
	@Get
	public Object get(Invocation inv) {
		
		int userId = 1000;
		if (hostHolder.getUser() != null) {
			userId = hostHolder.getUser().getId();
		}
		
		Blog b = new Blog();
		b.setOwnerId(userId);
		b.setTitle("Hello world");
		b.setContent("Good day~ Ladies and 乡亲们");
		
		return b;
		//return "@{'ownerId':1234, 'title':'Hello world', 'content':'Good day~ Ladies and 乡亲们'}".replace('\'',	'"');
	}
	
	@Get("blogs")
	public String gets(Invocation inv) {
		
		if (hostHolder != null) {
			System.out.println("host:" + hostHolder.getUser());
		}
		
		return ("@[{'ownerId':1234, 'title':'Hello world', 'content':'Good day~ Ladies and 乡亲们'}, " +
				"{'ownerId':5678, 'title':'ok', 'content':'好好学习天天向上'}]").replace('\'',	'"');
	}
	
	@Post
	public String post(Invocation inv) {
		inv.getResponse().addHeader("xoa-biz-code", "444");
		return "xoa-err:444;msg";
	}
	
	@Put
	public String put() {
		
		return null;
	}
	
	@Delete
	public String delete() {
		
		return null;
	}
}
