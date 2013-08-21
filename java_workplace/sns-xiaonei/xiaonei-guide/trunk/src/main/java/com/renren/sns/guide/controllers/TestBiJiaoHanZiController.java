package com.renren.sns.guide.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

@Path("tbjhz")
public class TestBiJiaoHanZiController {

	@Get
    public String index(Invocation inv) {
		inv.addModel("gpl","该评论");
		return "/views/guide/tbj.jsp";
	}
}
