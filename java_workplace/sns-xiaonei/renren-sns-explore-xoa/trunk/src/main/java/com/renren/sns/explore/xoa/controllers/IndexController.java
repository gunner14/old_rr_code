package com.renren.sns.explore.xoa.controllers;

import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

@Path("")
public class IndexController {

	@Get("")
	public String index(){
		return "@aa";
	}
	
}
