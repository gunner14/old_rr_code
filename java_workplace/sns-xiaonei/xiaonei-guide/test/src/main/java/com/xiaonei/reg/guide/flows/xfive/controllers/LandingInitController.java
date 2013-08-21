package com.xiaonei.reg.guide.flows.xfive.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.access.track.decision.Decision;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path({ "gameland", "landing" })
@IgnoreUserStatusImportedValidation
public class LandingInitController {
	private final static int thisStep = StepStatusLogic.STEP_LANDING_PAGE;
	private final static String INIT_GAME = "r:GuideForwardGames.do?ref=lp_game_arrival";
	@SuppressWarnings("unused")
	private final static String INIT_PAGE = "r:pagelanding?ref=lp_page_arrival";
	private final static String INIT_SOCIAL = "r:guide.do";
	private final static String INIT_OLD_GUIDE = "r:guide.do";
	
	private final static int SOCIAL_GRAPH_NOTSET = 0;
	private final static int SOCIAL_GRAPH_GOOD = 1;
	private final static int SOCIAL_GRAPH_BAD = 2;
	
	private static final String LANDING_AGARIC = "f:ld-agaric";
	
	@Get
    @Post
	public String post(Invocation inv, Decision dec){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null){
			GuideLogger.printLog(" request null ",GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
			return "@host null ";
		}
		else{
			try {
			    
			    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
	                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
	                return "r:http://reg.renren.com/sb/fillemail";
	            }
			    
				int type = StepProcessLogic.getInstance().getGuideUserType(host);
				GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
				switch(type){
				case IGuideUserLeadType.SKDefault:
					return LANDING_AGARIC;
				default:
					break;
				}
			}
			catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep+" e:"+e.toString(),GuideLogger.ERROR);
			}
				
			//改标志
			StepStatusLogic.updateStepStatus(host.getId(), thisStep);
			GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
		}
		
		
		int userdec = GuideXFiveUserDecisionLogic.getInstance().getDecision(host.getId());
		
		switch(userdec){
		case SOCIAL_GRAPH_NOTSET:
			GuideLogger.printLog(" host:"+host.getId()+" sgnotset");
			break;
		case SOCIAL_GRAPH_GOOD:
			try {
				GuideLogger.printLog(" host:"+host.getId()+" sggood");
				dec.makeDecision("sggood");
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" case good err:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			return INIT_SOCIAL;
		case SOCIAL_GRAPH_BAD:
			try {
				GuideLogger.printLog(" host:"+host.getId()+" sgbad");
				dec.makeDecision("sgbad");
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" case bad err:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			//Choco版本加上 诗论的需求，去PageLanding
			//2009-12-14 17:13:02 因为所有尾号的都来了，所以要判断一下 不是NBX5的才去Page
			//2009-12-24 11:47:05 诗伦需求，没有去Page的了。只有去Game与不去Game的分别了。来到这儿的都是去Game的
			/*
			if(GuideXFiveUserFlagLogic.getInstance().isXFiveChocoUser(host) && !GuideUtil.isNetBarXFiveUser(host)){
				return INIT_PAGE;
			}
			*/

			return INIT_GAME;
		default:
			GuideLogger.printLog(" host:"+host.getId()+" sg? "+ userdec);
			break;
		}
		
		return INIT_OLD_GUIDE;
		
	}
}
