package com.renren.xoa.demo.controllers.window;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;

public class TodoController {

	@Get
    public String xxx(Invocation inv) {
		return "@查bug-";
    }
	
}
