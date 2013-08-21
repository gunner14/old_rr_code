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
@Path("portraits")
@IgnoreUserStatusImportedValidation
public class PortraitSubmitController {
	private static final String SUCC = "f:lead"; 
	private final int thisStep = StepStatusLogic.STEP_UPLOAD_HEAD;
	
	@Get
	@Post
	public String get(Invocation inv){
		HttpServletRequest request = inv.getRequest();
		GuideTimeCost cost = GuideTimeCost.begin();
		final User host = BaseThreadUtil.currentHost(request);
		
		if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
		
		cost.endStep();
		GuideLogger.printLog(" host:"+host.getId());
		cost.endStep();
		//改标志
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
		cost.endStep();
		cost.endFinally();
		
		//这段估计要先停掉一段时间inshion
		/*
		try {
			//开心的别发新鲜事
			if(!"renren.com".equals(OpiConstants.domain.toString())){
				
			}
			else{
				Runnable task_tell = new Runnable() {
					public void run() {
						String hurl = host.getTinyUrl();
						if(durl1.equals(hurl) || durl2.equals(hurl)){
							//has head
						}
						else{
							GuideLogger.printLog(" host:" + host.getId() + " telling...");
							GuideXFiveFriendNewsLogic.getInstance().tellAll(host);
						}
					}
				};
				RunnableTaskExecutor.getInstance().runTask(task_tell);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		*/
		
		
		return SUCC;
	}
	@SuppressWarnings("unused")
	private static final String durl1 = "0/0/women_tiny.gif";
	@SuppressWarnings("unused")
	private static final String durl2 = "0/0/men_tiny.gif";
	
	
	
	
}
