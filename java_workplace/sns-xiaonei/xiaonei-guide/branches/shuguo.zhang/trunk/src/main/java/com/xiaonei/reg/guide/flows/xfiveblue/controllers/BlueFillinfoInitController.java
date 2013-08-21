package com.xiaonei.reg.guide.flows.xfiveblue.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUtil;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path("fiblue-old")
public class BlueFillinfoInitController {
	private static final String SUCC = "/views/guide/flowxfiveblue/fillinfo.jsp";
	
	@Get
    @Post
	public String get(Invocation inv){
		HttpServletRequest request = inv.getRequest();
		GuideLogger.printLog("");
		try {
			GuideXFiveUtil.setInitAttributes(request);
		} catch (Exception e) {
			GuideLogger.printLog("setInitAttributes failed! err "+e.toString()+GuideLogger.ERROR);
			e.printStackTrace();
		}
		return SUCC;
	}
	
}
