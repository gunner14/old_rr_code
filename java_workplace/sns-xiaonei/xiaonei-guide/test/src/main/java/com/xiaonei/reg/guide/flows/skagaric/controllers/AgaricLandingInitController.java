package com.xiaonei.reg.guide.flows.skagaric.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

@LoginRequired
@Path("ld-agaric")
public class AgaricLandingInitController {
	private final static int thisStep = StepStatusLogic.STEP_LANDING_PAGE;
	private final static String INIT = "r:guideka.do?ref=kaixin_app_guide_arrival";
	@Get
	@Post
	public String index(Invocation inv){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{	
			GuideTimeCost cost = GuideTimeCost.begin();
			//改标志
			StepStatusLogic.updateStepStatus(host.getId(), thisStep);
			GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
			cost.endStep();
			cost.endFinally();
			return INIT;
		}
		return INIT;
	}
}
