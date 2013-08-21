package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.IfParamExists;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;

public class TestController {

	@IfParamExists("ip=4")
	@Get("/{regionId:\\d+}")
    public Object index(Invocation inv, @Param("regionId") int regionId) {
        return "@" + regionId;
    }
    
    @Get("/ip={ip:\\d+\\.\\d+\\.\\d+\\.\\d+}")
    public Object index(Invocation inv, @Param("ip") String ip) {
    	return "@" + ip;
    }
}
