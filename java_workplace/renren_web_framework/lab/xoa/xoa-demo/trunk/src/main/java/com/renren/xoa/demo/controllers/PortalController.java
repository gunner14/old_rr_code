package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

public class PortalController {

	@Get
    public String home(Portal portal) throws Exception {
        portal.addWindow("weather", "/window/weather");
        portal.addWindow("todo", "/window/todo");
        portal.addWindow("todo", "/window/subportal");
        return "@portal-home";
    }
	
}
