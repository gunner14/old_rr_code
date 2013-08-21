package com.renren.xoa.demo.controllers;

import java.util.HashMap;
import java.util.Map;

import com.renren.xoa.commons.test.Blog;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

public class MapController {
	
	public MapController() {

	}

	@Get("string")
	@Put("string")
	@Post("string")
	@Delete("string")
	public Object string(Invocation inv) {
		Map<String, String> map = new HashMap<String, String>();
		map.put("aaa", "bbb");
		map.put("ccc", "ddd");
		map.put("eee", "fff");
		return map;
	}

	@Get("int")
	@Put("int")
	@Post("int")
	@Delete("int")
	public Object integer(Invocation inv) {
		Map<String, Integer> map = new HashMap<String, Integer>();
		map.put("aaa", 1);
		map.put("ccc", 2);
		map.put("eee", 3);
		return map;
	}
	
	@Get("blog")
	@Put("blog")
	@Post("blog")
	@Delete("blog")
	public Object blog(Invocation inv) {
		Map<String, Blog> map = new HashMap<String, Blog>();
		Blog a = new Blog();
		a.setTitle("a");
		a.setContent("aaa");
		a.setOwnerId(1);
		map.put("aaa", a);
		
		Blog b = new Blog();
		b.setTitle("b");
		b.setContent("bbb");
		b.setOwnerId(2);
		map.put("ccc", b);
		
		Blog c = new Blog();
		c.setTitle("c");
		c.setContent("ccc");
		c.setOwnerId(3);
		map.put("eee", c);
		return map;
	}
	
}
