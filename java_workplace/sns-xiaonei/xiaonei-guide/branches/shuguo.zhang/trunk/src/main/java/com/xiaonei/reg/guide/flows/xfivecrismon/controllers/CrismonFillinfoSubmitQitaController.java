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
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormQita;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;


@LoginRequired
@Path("ficrimsons-qt")
public class CrismonFillinfoSubmitQitaController {
	private final static String INIT = "f:ficrimson";
	private final static int thisStep = StepStatusLogic.STEP_FILLINFO | StepStatusLogic.STEP_UPLOAD_HEAD | StepStatusLogic.STEP_PREVIEW;
	
	@Get 
	public String get(Invocation inv){
		return INIT;
	}
	
	public final String SUCC = "f:lead";
	
	@Post
	public String post(Invocation inv,@Param("formQita")FormQita form){
		
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
				//String cr = checkAntiSpam();
				GuideTimeCost cost = GuideTimeCost.begin();
				try {
					cost.endStep();
					FormQita fform = (FormQita) form;
					//GuideLogger.printLog(" host:"+host.getId()+MyRequestUtil.getBrowserInfo(request));
					cost.endStep();
					GuideLogger.printLog(" host:"+host.getId()+form.getInfo());
					
					//家乡
					GuideXFiveUserProfileLogic.getInstance().saveHomeRegionQita(host,fform);
					cost.endStep();
					
					//所在地
					GuideXFiveUserProfileLogic.getInstance().saveCurrentRegionAndNetQita(host,fform);
					cost.endStep();
					
					//阶段
					//写阶段很关键 要同步
					GuideLogger.printLog("saveStage: "+host.getId()+" stage:"+Stage.STAGE_NONE);
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_NONE);
					cost.endStep();
					
//					//发新鲜事
//					A090729Util.sendNewEvent(host);
					
					//加网吧的Page 
					//2010-1-4 18:39:58 换成同步调用 
					//2010-1-29 16:35:25去掉 陈周需求
					//GuideXFiveUserPageLogic.getInstance().beFansofPage(600002899, host);
					
					cost.endStep();
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
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
				    
				    
					//GuideLogger.printLog(" host:"+host.getId()+" change to NetBar");
					//RegFromSourceLogic.update(host.getId(), RegFromSourceLogic.FS_NETBAR);
					//TODO add apps!
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
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
					GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
			
		}

		return ret;
	}
}
