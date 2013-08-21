package com.xiaonei.login.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.passport.PassportAdapterFactory;

public class LoginNotifyAction extends Action {

	@Override
	public ActionForward execute(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String origURL = request.getParameter("origURL");
		try {
			origURL = origURL == null ? OpiConstants.urlHome + "/Home.do"
					: origURL;
			request.setAttribute("origURL", origURL);
			String from = OpiConstants.isKaixin() ? "renren.com" : "kaixin.com";
			String to = OpiConstants.getCurrentDomain();
			request.setAttribute("from", from);
			request.setAttribute("to", to);
			OpiConstants.setCurrentDomain(from);
			request.setAttribute("fromname", OpiConstants
					.getDomainIdentifiedProperty("messages.domain.name"));
			OpiConstants.setCurrentDomain(to);
			request.setAttribute("toname", OpiConstants
					.getDomainIdentifiedProperty("messages.domain.name"));
			Integer hostid = PassportManager.getInstance().getHostIdFromCookie(
					request, response);
			if (!request.getMethod().toLowerCase().equals("post")) {
				// 直接过来的
				if (hostid != null && hostid > 0) {
					request.setAttribute("host", SnsAdapterFactory
							.getUserAdapter().get(hostid));
				}
				return mapping.findForward("fail");
			}
			if (hostid != null && hostid > 0) {
				String known = request.getParameter("known");
				if (known != null && known.equalsIgnoreCase("on")) {
					PassportAdapterFactory.getWebLogin().setStatus(hostid);
					System.err.println(known);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return new ActionForward(origURL, true);
	}
}
