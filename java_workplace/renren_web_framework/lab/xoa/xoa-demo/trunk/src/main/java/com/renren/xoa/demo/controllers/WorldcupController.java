package com.renren.xoa.demo.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;

public class WorldcupController {

	@Get("2010")
	public Object go2010(Invocation inv) {
		
		HttpServletRequest request = inv.getRequest();
		String param = request.getParameter("param");
		String msg = "param=";
		if (param == null) {
			msg += "null"; 
		} else if (param.length() == 0) {
			msg += "\"\"";
		} else {
			msg += param;
		}
		return "@2010-" + msg;
	}
	
	@Get("2006")
	public Object go2006() {
		return "@2006";
	}
	
	@Get("{year://d}")
	public Object gogogo() {
		return "@2006";
	}
	
}
