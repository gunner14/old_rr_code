package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * AjaxSubmitStageController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明 
 */
@LoginRequired
@Path("axsmtstg.do")
public class AjaxSubmitStageController{

	@Post
	@Get
	public String post(Invocation inv){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			String str_stage = request.getParameter("fi_stage");
			if(str_stage==null || str_stage.equals("")){
				JSONObject jo = new JSONObject();
				jo.put("msg", "no fi_stage in request");
				writeAjax(response, jo.toString());
				return null;
			}
			
			int stage = 0;
			try {
				stage = Integer.parseInt(str_stage);
				
			} catch (Exception e) {
				JSONObject jo = new JSONObject();
				jo.put("msg", "stage is not a number:"+stage);
				writeAjax(response, jo.toString());
				return null;
			}
			
			if(stage == 0){
				JSONObject jo = new JSONObject();
				jo.put("msg", "stage is 0");
				writeAjax(response, jo.toString());
				return null;
			}
			
			int defined_stage = 0;
			switch(stage){
			case 10:
				defined_stage = Stage.STAGE_MAJOR;
				break;
			case 20:
				defined_stage = Stage.STAGE_HEIGHSCHOOL;
				break;
			case 30:
				defined_stage = Stage.STAGE_UNIV;
				break;
			case 90:
				defined_stage = Stage.STAGE_NONE;
				break;
			default:
				JSONObject jo = new JSONObject();
			jo.put("msg", "stage is not a defined stage:"+str_stage);
			writeAjax(response, jo.toString());
			return null;
			}
			
			FillInfoLogic.getInstance().saveStage(host, defined_stage);
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
