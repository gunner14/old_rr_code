package com.xiaonei.reg.guide.flows.mobile.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

@LoginRequired
@Path("skipve")
public class SkipVerifyEmailController {
	private final int thisStep = StepStatusLogic.STEP_VERIFY_MOBILE | StepStatusLogic.STEP_COMPLETE_GUIDE;
	//private final static String SUCC = "r:/guide.do";
	private final static String SUCC = "/views/guide/endpoint/skipve.jsp";
	
	@Post
	public String post(Invocation inv){
		GuideLogger.printLog(" - start ");
		GuideLogger.printLog(" - end to get ");
		return get(inv);
	}
	
	@Get
	public String get(Invocation inv){
		GuideLogger.printLog(" - start ");
		GuideTimeCost cost = GuideTimeCost.begin();
		HttpServletRequest request = inv.getRequest();
		
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
		
		GuideLogger.printLog(" - end ");
		cost.endStep();
		cost.endFinally();
		return SUCC;
	}
}
