package com.xiaonei.reg.guide.action.ajaxforflashflow;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.logic.FillInfoLogic;

public class AjaxSubmitStage extends ProtectAction{

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		User host = BaseThreadUtil.currentHost(request);
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
		return null;
	}
	
	private ActionForward writeAjax(final HttpServletResponse response,
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
