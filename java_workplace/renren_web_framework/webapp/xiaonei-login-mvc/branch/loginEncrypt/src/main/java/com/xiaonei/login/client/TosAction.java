package com.xiaonei.login.client;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.component.application.cache.AppCacheMgr;
import com.xiaonei.platform.component.application.home.AppBasicHome;
import com.xiaonei.platform.component.application.home.AppUserAuthorizedHome;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.AppUserAuthorized;
import com.xiaonei.platform.component.application.model.AuthToken;
import com.xiaonei.platform.component.application.sk.mgr.SessionKeyManager;
import com.xiaonei.platform.component.application.sk.model.SessionKey;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.BaseAction;

/**
 * TosAction
 * 
 * @author lifs
 */
public class TosAction extends BaseAction {

	public ActionForward executeBt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		User host = BaseThreadUtil.currentHost(request);
		String ak = request.getParameter("api_key");
		String ver = request.getParameter("v");
		request.setAttribute("api_key", ak);
		request.setAttribute("api_version", ver);
		if (ak == null || ak.trim().length() == 0) {
			return mapping.findForward("fail");
		}
		AppBasic ab = AppBasicHome.getInstance().getByApiKey(ak);
		if (ab == null) {
			addErr(request, "传入的api_key不是校内提供的");
			return mapping.findForward("fail");
		}
		if (!"post".equalsIgnoreCase(request.getMethod())) {
			return mapping.findForward("fail");
		}
		// 验证用户是否接受了TOS
		String agreetos = request.getParameter("agreetos");
		if (agreetos != null && agreetos.equals("yes")) {
			// 生成token,存入au对象中，供接口调用
			String authToken = request.getParameter("auth_token");
			SessionKey sk = SessionKeyManager.getInstance().getSessionKey(
					SessionKeyManager.SESSION_KEY_CANVASDESKTOP, host.getId(),
					ab.getAppId(), true);
			AuthToken au = AppCacheMgr.getAuthToken(authToken);
			if (au != null) {
				au.setSessionKey(sk.getSessionKey());
				au.setSessionSecret(sk.getSessionSecret());
				au.setUserId(host.getId());
				AppCacheMgr.setAuthToken(authToken, au);
			}
			// 同意tos后，app和user建立了关联
			AppUserAuthorized aua = new AppUserAuthorized();
			aua.setAppId(ab.getAppId());
			aua.setUserId(host.getId());
			// aua.setKeepLogged(keepLogged);
			AppUserAuthorizedHome.getInstance().save(aua);
			String cbu = ab.getCallbackUrl();
			request.setAttribute("auth_token", authToken);
			request.setAttribute("callback_url", cbu);
			return mapping.findForward("succ");
		} else {

			return mapping.findForward("fail");
		}
	}

}