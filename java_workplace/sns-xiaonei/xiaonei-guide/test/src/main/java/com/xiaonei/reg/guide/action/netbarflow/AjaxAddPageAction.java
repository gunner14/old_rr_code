package com.xiaonei.reg.guide.action.netbarflow;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.util.GuideLogger;

public class AjaxAddPageAction extends ProtectAction{

	//这个Action从AccessLog里看是没被访问过的。
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		MyTimeCost cost = MyTimeCost.begin();
		final User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:"+host.getId());
		cost.endStep();
		// 用户id组
		/*
		String user_id = request.getParameter("pages");
		cost.endStep();
		String[] user_ids = { "" };
		cost.endStep();
		if (user_id != null && !"".equalsIgnoreCase(user_id)) {
			user_ids = user_id.split(",");
			cost.endStep();
		}
		final String[] ids = user_ids;
		
		Runnable task = new Runnable(){
			@Override
			public void run() {
				for (String id : ids){
					A090729Util.beFansofPage(id, host);
				}
			}
		};
		
		RunnableTaskExecutor.getInstance().runTask(task);
		 */
		
		cost.endStep();
		writeAjax(response,"OK");
		
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
