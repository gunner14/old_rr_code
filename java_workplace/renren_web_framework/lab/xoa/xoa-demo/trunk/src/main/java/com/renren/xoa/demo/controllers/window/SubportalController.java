package com.renren.xoa.demo.controllers.window;

import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

public class SubportalController {

	@Get
    public String subportal(Portal portal) throws Exception {
        portal.addWindow("weather", "/window/weather");
        portal.addWindow("todo", "/window/todo");
        return "@subportal";
    }
}
