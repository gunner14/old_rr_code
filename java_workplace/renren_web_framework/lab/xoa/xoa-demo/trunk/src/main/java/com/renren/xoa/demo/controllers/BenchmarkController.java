package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;

import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.commons.test.BlogFactory;

public class BenchmarkController {

	private Logger logger = Logger.getLogger(this.getClass());
	
	//@Autowired
	//private HostHolder hostHolder;
	
	private BlogFactory blogFactory = BlogFactory.getInstance();
	
	@Get("/{k}/{delay}")
	@Post("/{k}/{delay}")
	public Object index(Invocation inv, @Param("k")Integer k, @Param("delay")Integer delay) {
		
		Blog[] blogs = blogFactory.getDataBySize(k);
		if (delay > 0) {
			try {
				Thread.sleep(delay);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		return blogs;
	}
}
