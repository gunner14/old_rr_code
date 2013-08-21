package com.xiaonei.commons.controllers;

import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;


@Path("")
public class HealthyController {

	
	
	@Get("healthy")
	public Object healthy() {
		return "@OK";
	}
	
}
