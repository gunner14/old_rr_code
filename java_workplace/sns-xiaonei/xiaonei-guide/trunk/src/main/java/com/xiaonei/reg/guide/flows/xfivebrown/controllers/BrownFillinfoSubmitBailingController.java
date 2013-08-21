package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;


@LoginRequired
@Path("fibrowns-bl")
@IgnoreUserStatusImportedValidation
public class BrownFillinfoSubmitBailingController {
	private final static String INIT = "f:fibrown";
	private final static int thisStep = StepStatusLogic.STEP_FILLINFO;
	
	@Get
	public String get(Invocation inv){
		return INIT;
	}
	
	
	public final String SUCC = "f:lead";
	
	@Post
	public String post(Invocation inv,CoralFormInfo form){
		
		String ret = INIT;
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null){
			GuideLogger.printLog(" request null ",GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		
		else{
			boolean isdone = GuideXFiveStepCheckLogic.getInstance().isTheStepDone(thisStep, host);
			if(isdone){
				ret = SUCC;
			}
			else{
				GuideTimeCost cost = GuideTimeCost.begin();
				try {
					request.setAttribute("action_form", form);
					cost.endStep();
					//String cr = checkAntiSpam();
					final CoralFormInfo fform = (CoralFormInfo) form;
					cost.endStep();
					cost.endStep();
					GuideLogger.printLog("sb() - hostid:"+ host.getId()+ " - start - " + fform.toString());
					
					//工作
					GuideXFiveUserInfoLogic.getInstance().addWork(host, fform);
					cost.endStep();
					//家乡
					
					GuideXFiveUserInfoLogic.getInstance().saveHomeRegion(host, fform);
					cost.endStep();
					
					//所在地
					GuideXFiveUserInfoLogic.getInstance().saveCurrentRegionAndNet(host, fform);
					cost.endStep();
					
					//大学
					GuideXFiveUserInfoLogic.getInstance().saveUniversity(host, fform);
					cost.endStep();
					
					//高中
					GuideXFiveUserInfoLogic.getInstance().saveHighschool(host, fform);
					
					//阶段
					//写阶段很关键 要同步
					GuideLogger.printLog("saveStage: "+host.getId()+" stage:"+Stage.STAGE_MAJOR);
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_MAJOR);
					cost.endStep();
					
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				try {
					//改标志
					StepStatusLogic.updateStepStatus(host.getId(), thisStep);
					GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
					cost.endStep();
					cost.endFinally();
					ret = SUCC;
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				
			}
		}
		return ret;
	}
}
