package com.xiaonei.reg.guide.flows.xfive.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

@LoginRequired
@Path("imports")
@IgnoreUserStatusImportedValidation
public class ImportmsnSubmitController {
	private static final String SUCC = "f:lead"; 
	private static final int thisStep = StepStatusLogic.STEP_IMPORT_MSN;
	
	@Get
	@Post
	public String index(Invocation inv){
		HttpServletRequest request = inv.getRequest();
		GuideTimeCost cost = GuideTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		
		if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
		
		cost.endStep();
		GuideLogger.printLog("host:"+host.getId());
		cost.endStep();
		
		//改标志
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
		cost.endStep();
		cost.endFinally();
		if("l".equals(request.getParameter("f"))){
			return "r:lead";
		}
		return SUCC;
	}
	
	
}
