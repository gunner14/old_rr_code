package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;

public class DemoController {

	@Get("")
	public Object index(Invocation inv) {
		System.out.println("ININININININININ");
		return "@haha";
	}
	
}
