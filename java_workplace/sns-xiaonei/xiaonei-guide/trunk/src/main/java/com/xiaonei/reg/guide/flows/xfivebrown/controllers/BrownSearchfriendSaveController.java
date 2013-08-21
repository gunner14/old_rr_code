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
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoCheckLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;
import com.xiaonei.reg.guide.root.interfaces.IXProfilable;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;

@LoginRequired
@Path("sfbrown-s-r")
@IgnoreUserStatusImportedValidation
public class BrownSearchfriendSaveController {
	
	@Get
	@Post
	public String index(Invocation inv,CoralFormInfo form){
		
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
			String jret = null;
			
			String tname = GuideXFiveRequestUtil.getParamString(request, "targetname", host.getId());
			Map<String,IXProfilable> param =  new HashMap<String, IXProfilable>();
			
			String checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAUniversity(form);
			if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){
				param.put("info_dx", form);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
			}
			checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isASeniorhighschool(form);
			if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){
				param.put("info_gz", form);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
			}
			checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAJuniorhighschool(form);
			if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){
				param.put("info_cz", form);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
			}
			checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isATechschool(form);
			if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){
				param.put("info_jx", form);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
			}
			checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAWorkInfo(form);
			if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){
				param.put("info_gs", form);
			}
			checkrst = GuideXFiveUserInfoCheckLogic.getInstance().isAElementaryschool(form);
			if(GuideXFiveUserInfoCheckLogic.RET_INFO_OK == checkrst){
				param.put("info_xx", form);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" "+checkrst);
			}
			
			if(MyStringUtils.isNullOrEmpty(tname) && param.size() == 0){
				return "err: no condition! need at least 1...";
			}
			List<Integer> list = GuideXFiveIDsPreviewLogic.getInstance().getSearchfriendResults(150, host, tname, param);
			jret = GuideUtil.getJSONForFriendSelectorFromIdsForSearchfriend(list);
			
			GuideXFiveRequestUtil.writeAjax(response,jret);
		}
		return "@";
	}
	
	
}
