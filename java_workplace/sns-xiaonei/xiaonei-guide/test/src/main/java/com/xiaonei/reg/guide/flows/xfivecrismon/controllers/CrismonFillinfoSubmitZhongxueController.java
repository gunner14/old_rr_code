package com.xiaonei.reg.guide.flows.xfivecrismon.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.admin.biz.model.school.SchoolType;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormZhongxue;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveStepCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;


@LoginRequired
@Path("ficrimsons-zx")
public class CrismonFillinfoSubmitZhongxueController {
	private final static String INIT = "f:ficrimson";
	private final static int thisStep = StepStatusLogic.STEP_FILLINFO;
	
	@Get 
	public String get(Invocation inv){
		return INIT;
	}
	
	public final String SUCC = "f:lead";
	
	@Post
	public String post(Invocation inv,@Param("formZhongxue")FormZhongxue form){
		
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
				FormZhongxue fform = (FormZhongxue) form;
				GuideLogger.printLog(" host:"+ host.getId()+ " - start");
				try {
					GuideLogger.printLog(" host:"+"Schooltype int is "+fform.getSchooltype()+" "+fform.getHighschoolname()+" "+fform.getJuniorhighschoolname()+" "+fform.getElementaryschoolname()+" "+fform.getTechschoolname());
					GuideLogger.printLog(" host:"+"class "+fform.getHighschoolclass1());
					GuideLogger.printLog(" host:"+"info "+fform.getInfo());
					int stype = Integer.valueOf(fform.getSchooltype()) + 2;
					boolean iscurrentnetvalid = checkMsNet(fform,stype);
					switch (stype) {
					case SchoolType.TYPE_HIGHSCHOOL:
						GuideLogger.printLog(" host:"+ host.getId()+ " - TYPE_HIGHSCHOOL");
						
						GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolZhongxue(host, fform);
						GuideXFiveUserProfileLogic.getInstance().saveHighschoolZhongxue(host, fform);
						if(iscurrentnetvalid){
							GuideXFiveUserProfileLogic.getInstance().saveHighschoolNetZhongxue(host, fform);
						}
						break;
					case SchoolType.TYPE_JUNIORSCHOOL:
						GuideLogger.printLog(" host:"+ host.getId()+ " - TYPE_JUNIORSCHOOL");
						
						GuideXFiveUserProfileLogic.getInstance().saveElementaryschoolZhongxue(host, fform);
						GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolZhongxue(host, fform);
						if(iscurrentnetvalid){
							GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolNetZhongxue(host, fform);
						}
						break;
					case SchoolType.TYPE_COLLEGESCHOOL:
						GuideLogger.printLog(" host:"+ host.getId()+ " - TYPE_COLLEGESCHOOL");
						GuideXFiveUserProfileLogic.getInstance().saveJuniorschoolZhongxue(host, fform);
						GuideXFiveUserProfileLogic.getInstance().saveCollegeZhongxue(host, fform);
						if(iscurrentnetvalid){
							GuideXFiveUserProfileLogic.getInstance().saveCollegeNetZhongxue(host, fform);
						}
						break;
					}
					
				} catch (RuntimeException e) {
					GuideLogger.printLog(" host:"+"sb() error zhongxue "+e.getMessage(),1);
				}
				
				try {
					GuideLogger.printLog(" host:"+ "sb() - hostid:"+ host.getId()+ " - savestage - stage:"+Stage.STAGE_HEIGHSCHOOL);
					//阶段
					FillInfoLogic.getInstance().saveStage(host, Stage.STAGE_HEIGHSCHOOL);
					
//					//2009-11-26 15:37:36 诗伦让去掉
//					//新手帮
//					A090729Util.asyncBeFansofPage(600002351, host);
					
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+"sb() saveStage error "+host.getId()+" "+e.toString(),1);
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
					GuideLogger.printLog("| host:"+host.getId()+" update step:"+thisStep);
					ret = SUCC;
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}

		return ret;
	}
	
	private boolean checkMsNet(FormZhongxue fform,int type) {
		//int type = Integer.valueOf(fform.getSchooltype()) + 2;
		switch (type) {
		case SchoolType.TYPE_HIGHSCHOOL: //gaozhong 
			if (StringUtils.isNotEmpty(fform.getHighschoolname())){
				GuideLogger.printLog("checkMsNet() high true");
				return true;
			}
			break;
		case SchoolType.TYPE_JUNIORSCHOOL: //chuzhong
			if (StringUtils.isNotEmpty(fform.getJuniorhighschoolname())){
				GuideLogger.printLog("checkMsNet() juni true");
				return true;
			}
			break;
		case SchoolType.TYPE_COLLEGESCHOOL: //jixiao
			if (StringUtils.isNotEmpty(fform.getTechschoolname())){
				GuideLogger.printLog("checkMsNet() tech true");
				return true;
			}
			break;
		}
		GuideLogger.writeLine(this,"checkMsNet() false schooltype = "+type );
		return false;
	}

}
