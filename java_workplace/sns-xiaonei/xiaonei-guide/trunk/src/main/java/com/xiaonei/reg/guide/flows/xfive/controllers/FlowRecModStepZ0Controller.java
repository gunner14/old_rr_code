package com.xiaonei.reg.guide.flows.xfive.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * FlowRecommendInterestController.java
 * @author 董刚锋(gangfeng.dong@renren-inc.com)
 * 2012-03-12
 */
@LoginRequired
@Path({"flowrecommendmodifystep"})
@IgnoreUserStatusImportedValidation
public class FlowRecModStepZ0Controller {
    
   
    @Get
    @Post
    public String get(Invocation inv){
    	 User host = GuideXFiveRequestUtil.getCurrentHost(inv.getRequest()); 
    	 try {
				//改标志
				StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FlOW_RECOMMEND);
				GuideLogger.printLog("| host:"+host.getId()+" update step:"+StepStatusLogic.STEP_FlOW_RECOMMEND);
			} catch (Exception e) {
				e.printStackTrace();
			}
    	 return "f:lead";
    }
}
