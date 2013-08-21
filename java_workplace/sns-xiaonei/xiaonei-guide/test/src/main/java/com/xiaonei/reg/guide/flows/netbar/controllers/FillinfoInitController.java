package com.xiaonei.reg.guide.flows.netbar.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path("finb")
public class FillinfoInitController {
	
    @Get
    public String get(Invocation inv){
		
		GuideLogger.printLog(" - start ");
		HttpServletRequest request = inv.getRequest();
		//保存基本的 年龄和名字
		User host = BaseThreadUtil.currentHost(request);
		/*
		try {
			UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
			request.setAttribute("user_name", host.getName());
			request.setAttribute("user_age", ub.getBirthYear());
			
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" set name or age error "+e.toString(),GuideLogger.ERROR);
			
		}
		
		//看是否是17岁以上，关系到引导导航条
		try {
			Boolean is_less_17 = false;
			cost.endStep();
			request.setAttribute("is_less_17", is_less_17);
			
			Boolean is_app_guide = false;
			request.setAttribute("is_app_guide", is_app_guide);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" set 2 error "+e.toString(),GuideLogger.ERROR);
		}
		cost.endStep();
		cost.endFinally();
		
		GuideLogger.printLog(" - end ");
		*/
		GuideLogger.printLog("WHY? host:"+host.getId(),GuideLogger.ERROR);
		return getReturnPath("fillinfo");
	}
	
	
	private String getReturnPath(String pagename){
		return prefix + pagename + postfix;
	}
	
	private final static String prefix = "/views/guide/flownetbar/";
	private final static String postfix = ".jsp";
	
	public static void main(String[] args) {
		FillinfoInitController f = new FillinfoInitController();
		System.out.println(f.getReturnPath("fillinfo"));
	}
}
