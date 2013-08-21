package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.reg.guide.util.GuideStatLogUtil;
import org.apache.log4j.Logger;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;

/**
 * AjaxUserTraceLogController.java
 * @author ji.wei
 * 
 * 统计guide流程中推荐好友的情况
 * 
 * 2010-9-25 添加说明 
 */

@LoginRequired
@Path({ "ajaxGuideUserTraceLog","ajaxUserTraceLog"})

public class AjaxUserTraceLogController {

	@Post
	@Get
	public String get(Invocation inv){
		final HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		String actionName=inv.getParameter("actionName");
		String logStr =inv.getParameter("log");
        GuideStatLogUtil.log("User:" + host.getId() + "|Action:" + actionName + "|" + logStr);
		return "@";	
	}

}
