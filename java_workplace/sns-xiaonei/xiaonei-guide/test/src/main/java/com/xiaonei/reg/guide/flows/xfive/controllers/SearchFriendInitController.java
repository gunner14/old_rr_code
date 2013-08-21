package com.xiaonei.reg.guide.flows.xfive.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUtil;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path({"searchfriend"})
@IgnoreUserStatusImportedValidation
public class SearchFriendInitController {
	private static final String SUCC_DEFAULT = "f:sfbrown";//"/views/guide/flowxfivedark/searchfriend_dark.jsp";
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
			try {
			    
			    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
	                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
	                return "r:http://reg.renren.com/sb/fillemail";
	            }
			    
				GuideXFiveUtil.setInitAttributes(request);
				int type = StepProcessLogic.getInstance().getGuideUserType(host);
				GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
				switch(type){
				case IGuideUserLeadType.XFiveGeneral:
				case IGuideUserLeadType.XFiveBrown:
					return SUCC_DEFAULT;
				default:
					return SUCC_DEFAULT;
				}
				
			} catch (Exception e) {
				GuideLogger.printLog("setInitAttributes failed! err "+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			return SUCC_DEFAULT;
		}
		return SUCC_DEFAULT;
	}
}
