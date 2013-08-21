package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveIDsPreviewLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.interfaces.IXProfilable;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.IRegExtraMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic;
import com.xiaonei.reg.register.model.RegExtraMsg;

@LoginRequired
@Path("sfbrown")
@IgnoreUserStatusImportedValidation
public class BrownSearchfriendInitController {
	
	private static final String INIT = "/views/guide/flowxfivedark/searchfriend_dark.jsp";
	private static final String SKIP_DEFAULT = "f:lead"; 
	private final int thisStep = IStepStatusDefinition.STEP_SEARCH_FRIEND;
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
			Map<String, RegExtraMsg> map = null;
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				if( map == null){
					GuideLogger.printLog(" host:"+host.getId()+" map null");
					skip(host);
					return SKIP_DEFAULT;
				}
				RegExtraMsg msg = map.get(IRegExtraMsgLogic.key_reg_recommond);
				String targetname = "";
				if( msg == null){
					GuideLogger.printLog(" host:"+host.getId()+" msg null");
					skip(host);
					return SKIP_DEFAULT;
				}
				else{
					targetname = msg.getInput_value();
					if(MyStringUtils.isNullOrEmpty(targetname)){
						GuideLogger.printLog(" host:"+host.getId()+" targetname:"+targetname);
						skip(host);
						return SKIP_DEFAULT;
					}
					else if(MyStringUtils.contains(targetname, "姓名")){
						GuideLogger.printLog(" host:"+host.getId()+" targetname:"+targetname);
						skip(host);
						return SKIP_DEFAULT;
					}
					else{
						List<Integer> list = GuideXFiveIDsPreviewLogic.getInstance().getSearchfriendResults(150, host, targetname, new HashMap<String, IXProfilable>());
						if(list == null || list.size() == 0){
							GuideLogger.printLog(" host:"+host.getId()+" targetname:"+targetname+" search:no ret");
							skip(host);
							return SKIP_DEFAULT;
						}
						
						String lead_search_friend = GuideUtil.getJSONForFriendSelectorFromIdsForSearchfriend(list);
						
						GuideLogger.printLog(" host:"+host.getId()+" targetname:"+targetname);
						inv.addModel("lead_target_name", targetname);
						inv.addModel("lead_search_friend", lead_search_friend);
					}
				}
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			
		}
		
		return INIT;
	}
	
	private void skip(User host) {
		GuideLogger.printLog(" host:"+host.getId()+" skip:"+thisStep);
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
	}
}
