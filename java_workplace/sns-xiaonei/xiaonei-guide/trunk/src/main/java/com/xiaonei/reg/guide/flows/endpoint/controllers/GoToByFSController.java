package com.xiaonei.reg.guide.flows.endpoint.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

/**
 * GoToByFSController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2011-2-26 新建文件 
 */
@LoginRequired
@Path("gotofs")
@IgnoreUserStatusImportedValidation
public class GoToByFSController {
	private final static String DEFAULT = "/views/guide/endpoint/gotofs.jsp";
	private final static String SUCC = "/views/guide/endpoint/gotofs.jsp";
	
	private final int thisStep = StepStatusLogic.STEP_COMPLETE_GUIDE;
	
	@Post
	@Get
	public String index(Invocation inv){
		String ret = DEFAULT;
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		if(request == null){
			GuideLogger.printLog(" request null ",GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			StepStatusLogic.updateStepStatus(host.getId(), thisStep);
			String whole_from_source = ""+RegFromSourceLogic.getFromSource(host);
			List<String> list = RegFromSourceLogic.getFromSoureList(whole_from_source);
			String the_important_fs = getTheImportantFs(list); 
			String fsid_extra = GuideUtil.getExtra_XXXString(host,the_important_fs);
			GuideLogger.printLog(" host:"+host.getId()+" tfs:"+the_important_fs+" fsid:"+fsid_extra);
			if(fsid_extra!=null && !fsid_extra.trim().equals("")){
				inv.addModel("the_important_fs",the_important_fs);
				inv.addModel("whole_from_source",whole_from_source);
				inv.addModel("fsid_extra",fsid_extra);
				ret = SUCC;
			}
		}
		return ret;
	}

	private String getTheImportantFs(List<String> list) {
		String ret = "";
		if(list != null){
			for (String string : list) {
				if(string!=null && string.startsWith("_")){
					ret = string.trim();
					break;
				}
			}
		}
		return ret;
	}
	
	public static void main(String[] args) {
		String s = (String)null;
		System.out.println(s);
	}
}
