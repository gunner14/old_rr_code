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
import com.xiaonei.reg.guide.logic.GuideUserTraceLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * 2010-8-23 10:23:30 改到XFiveURLTraceController
 * @author inshion(xin.yin@opi-corp.com)
 *
 */
public class AjaxUserTraceAction extends ProtectAction{

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		final User host = BaseThreadUtil.currentHost(request);
		String actionid = request.getParameter("i");
		if(actionid == null || actionid.trim().equals("")){
			GuideLogger.printLog(" host:"+host.getId() + "err actionid:["+actionid+"]",GuideLogger.ERROR);
			writeAjax(response,"actionid error");
			return null;
		}
		else{
			if(GuideUserTraceLogic.getInstance().checkNetbarActionId(actionid)){
				GuideLogger.printLog(" host:"+host.getId() + "netbar actionid:["+actionid+"]");
				GuideUserTraceLogic.getInstance().asyncWriteTrace(request, host, actionid);
			}
			else if(GuideUserTraceLogic.getInstance().checkBystargeActionId(actionid)){
				GuideLogger.printLog(" host:"+host.getId() + "stage actionid"+actionid+"]");
				GuideUserTraceLogic.getInstance().asyncWriteTrace(request, host, actionid);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId() + "inv actionid:["+actionid+"]",GuideLogger.ERROR);
				writeAjax(response,"actionid invalid");
				return null;
			}
		}
		GuideLogger.printLog(" host:"+host.getId()+" actionid:"+actionid);
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
