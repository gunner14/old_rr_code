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
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormDaxue;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;


@LoginRequired
@Path("ficrimsons-dx")
public class CrismonFillinfoSubmitDaxueController {
	private final static String INIT = "f:ficrimson";
	private final static int thisStep = StepStatusLogic.STEP_FILLINFO;
	
	@Get 
	public String get(Invocation inv){
		return INIT;
	}
	
	public final String SUCC = "f:lead";
	
	@Post
	public String post(Invocation inv,@Param("formDaxue")FormDaxue form){
		
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
				final FormDaxue fform = (FormDaxue) form;
				//GuideLogger.printLog(" host:"+host.getId()+MyRequestUtil.getBrowserInfo(request));
				//String cr = checkAntiSpam();
				cost.endStep();
				try {
					GuideLogger.printLog("sb() - hostid:"+ host.getId()+ " - start - " + fform.getInfo());
					
					cost.endStep();
					
					/*保存大学的逻辑改为以下新接口操作*/
					GuideXFiveUserProfileLogic.getInstance().addUniversityAndNetDaxue(host, fform);
					cost.endStep();

					
					/*保存中学的逻辑改为以下新接口操作*/
					GuideXFiveUserProfileLogic.getInstance().saveHighschoolDaxue(host, fform);
					cost.endStep();

					//写阶段很关键 要同步
					GuideLogger.printLog("saveStage: "+host.getId());
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_UNIV);
					cost.endStep();
					
					/*//2010-2-5 14:14:00 Inshion 去掉
					//09新生
					A090729Util.beFansof09(host);
					cost.endStep();*/
					
//					//2009-11-26 15:37:36 诗伦让去掉
//					//新手帮
//					A090729Util.asyncBeFansofPage(600002351, host);
//					cost.endStep();

					cost.endStep();
					GuideLogger.writeLine(this , "sb() - hostid:"+ host.getId()+ " - end - "+fform.getUniversitycode()+" | "+fform.getHighschoolcode());
					
					
					
				} catch (Exception e) {
					GuideLogger.writeLine(this, "sb() error "+host.getId()+" "+e.toString(),1);
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
				    
				    
				    
					//GuideLogger.printLog(" host:"+host.getId()+" change to NBX5");
					//RegFromSourceLogic.update(host.getId(), RegFromSourceLogic.FS_NETBARXFIVE);
				} catch (Exception e) {
					e.printStackTrace();
				}
				
				try {
					//改标志
					StepStatusLogic.updateStepStatus(host.getId(), thisStep);
					GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
					ret = SUCC;
					
					cost.endStep();
					cost.endFinally();
				} catch (Exception e) {
					e.printStackTrace();
				}
				
			}
			
		}

		return ret;
	}
}
