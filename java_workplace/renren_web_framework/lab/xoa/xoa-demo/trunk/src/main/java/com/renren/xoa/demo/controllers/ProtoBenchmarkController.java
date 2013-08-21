package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;

import com.renren.xoa.commons.test.RizhiFactory;
import com.renren.xoa.commons.test.RizhiProtos.RizhiList;

public class ProtoBenchmarkController {

	private Logger logger = Logger.getLogger(this.getClass());
	
	//@Autowired
	//private HostHolder hostHolder;
	
	private RizhiFactory rizhiFactory = RizhiFactory.getInstance();
	
	@Get("/{k}/{delay}")
	@Post("/{k}/{delay}")
	public Object index(Invocation inv, @Param("k")Integer k, @Param("delay")Integer delay) {
		
		RizhiList rizhis = rizhiFactory.getDataBySize(k);
		if (delay > 0) {
			try {
				Thread.sleep(delay);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		return rizhis;
	}
}
