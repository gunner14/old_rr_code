package com.renren.xoa.demo.controllers;


import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

public class EncodingTestController {

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv, String title, String body) throws Exception{
		HttpServletRequest request = inv.getRequest();
		request.setCharacterEncoding("UTF-8");
		String title1 = request.getParameter("title");
		String body1 = request.getParameter("body");
		System.out.println(title1);
		System.out.println(body1);
		/*System.out.println(title);
		System.out.println(body);*/
		//return "@" + title + "-" + body + "|" + title1 + "-" + body1;
		return "@" + title1;
	}
	
}
