package com.xiaonei.reg.register.controllers.ajax;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;

/**
 * 试用于分2步骤注册的页面，第一次点击按钮的事件记录 2008-8-28
 * 
 * @author wei.cheng@opi-corp.com
 */

@Path({ "AjaxRegisterLog" })
public class AjaxRegisterLogController {

	@Get
	@Post
	public String executeBt(Invocation invocation) {

		TimeCost cost = TimeCost.begin("AjaxRegisterAuthAction");

		HttpServletRequest request = invocation.getRequest();
		String email = request.getParameter("email");
		String actionIdStr = request.getParameter("action_id");
		String s1 = request.getParameter("s1");
		String s2 = request.getParameter("s2");
		String s3 = request.getParameter("s3");
		String s4 = request.getParameter("s4");
		int ivId = 0;
		try {
			try {
				if (request.getParameter("i") != null
						&& !request.getParameter("i").equals("")) {
					ivId = Integer.parseInt(request.getParameter("i"));
				}
			} catch (Exception e) {
			}
			cost.endStep("1");
			UserTraceLogic.getInstance().insertRegHis(request, null, email,
					actionIdStr, 0, s1, s2, s3, s4, ivId);
			cost.endStep("2");
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			cost.endFinally();
		}
		return null;
	}

}
