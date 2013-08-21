package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * AjaxGetUserStepStatusController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明 
 */
@LoginRequired
@Path("axgets.do")
public class AjaxGetUserStepStatusController{

	@Post
	@Get
	public String post(Invocation inv){
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
			
			int step = 0;
			try {
				step = StepStatusLogic.getStepStatus(host.getId());
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			GuideLogger.printLog("step is"+step+" - host:"+host.getId());
			
			String gostep = "0";
			if(step == 0 || (step & StepStatusLogic.STEP_FLASH_APP_OPEN) != StepStatusLogic.STEP_FLASH_APP_OPEN){
				gostep = "1";
			} else if ((step & StepStatusLogic.STEP_FILLINFO) != StepStatusLogic.STEP_FILLINFO) {
				gostep = "2";
			} else if ((step & StepStatusLogic.STEP_PREVIEW) != StepStatusLogic.STEP_PREVIEW) {
				gostep = "3";
			} else if ((step & StepStatusLogic.STEP_UPLOAD_HEAD) != StepStatusLogic.STEP_UPLOAD_HEAD) {
				gostep = "4";
			}
			JSONObject jo = new JSONObject();
			jo.put("gostep", gostep);
			writeAjax(response, jo.toString());
		}
		return null;
	}
	
	
	private String writeAjax(final HttpServletResponse response,
			final String str) {
		try {
			response.setContentType("text/html; charset=UTF-8");
			response.getWriter().print(str);
			response.getWriter().flush();
			response.getWriter().close();
		} catch (IOException e) {

		} finally {
		}
		return null;
	}
}
