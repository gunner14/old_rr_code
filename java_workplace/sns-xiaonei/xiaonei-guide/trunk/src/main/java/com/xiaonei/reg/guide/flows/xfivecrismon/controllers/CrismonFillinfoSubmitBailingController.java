package com.xiaonei.reg.guide.flows.xfivecrismon.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormBailing;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;


@LoginRequired
@Path("ficrimsons-bl")
public class CrismonFillinfoSubmitBailingController {
	private final static String INIT = "f:ficrimson";
	private final static int thisStep = StepStatusLogic.STEP_FILLINFO;
	
	@Get
	public String get(Invocation inv){
		return INIT;
	}
	
	public final String SUCC = "f:lead";
	
	@Post
	public String post(Invocation inv,@Param("formBailing")FormBailing form){
		
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
					final FormBailing fform = (FormBailing) form;
					cost.endStep();
					cost.endStep();
					GuideLogger.printLog("sb() - hostid:"+ host.getId()+ " - start - " + fform.getInfo());
					
					//工作
					GuideXFiveUserProfileLogic.getInstance().addWorkBailing(host, fform);
					cost.endStep();
					//家乡
					GuideXFiveUserProfileLogic.getInstance().saveHomeRegionBailing(host, fform);
					cost.endStep();
					
					//所在地
					GuideXFiveUserProfileLogic.getInstance().saveCurrentRegionAndNetBailing(host, fform);
					cost.endStep();
					
					//阶段
					//写阶段很关键 要同步
					GuideLogger.printLog("saveStage: "+host.getId()+" stage:"+Stage.STAGE_MAJOR);
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_MAJOR);
					cost.endStep();
					
				} catch (Exception e) {
					e.printStackTrace();
				}
				
				try {
					//改FromSource！又恐怖了啊！杯具…… Inshion 2009-12-14 16:29:17
				    
				    String fromsource = RegFromSourceLogic.getFromSource(host);
	                List<String> fromlist = RegFromSourceLogic.getFromSoureList(fromsource);
				    if(fromlist.contains(RegFromSourceLogic.FS_LEAD_ACB)){
				        GuideLogger.printLog("MTXX host:"+host.getId()+" Notchange "); 
				    }
				    else{
				        GuideLogger.printLog(" host:"+host.getId()+" change to NBX5");
	                    RegFromSourceLogic.update(host.getId(), RegFromSourceLogic.FS_NETBARXFIVE);
				    }
					
				} catch (Exception e) {
					e.printStackTrace();
				}
				
				try {
					//改标志
					StepStatusLogic.updateStepStatus(host.getId(), thisStep);
					GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
					cost.endStep();
					cost.endFinally();
					ret = SUCC;
				} catch (Exception e) {
					e.printStackTrace();
				}
				
				
			}
		}
		return ret;
	}
}
