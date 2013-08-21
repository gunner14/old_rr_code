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
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

public class AjaxRequestUserStepStatus extends ProtectAction{

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		User host = BaseThreadUtil.currentHost(request);
		int step = StepStatusLogic.getStepStatus(host.getId());
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
//		JSONArray ja = new JSONArray();
//		ja.add(jo);
		writeAjax(response, jo.toString());
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
