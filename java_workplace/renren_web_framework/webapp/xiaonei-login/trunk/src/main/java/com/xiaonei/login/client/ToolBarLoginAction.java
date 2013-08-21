package com.xiaonei.login.client;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.client.base.AbstractStructAction;
import com.xiaonei.login.client.base.LoginAction;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.task.set.FlyerTaskSet;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.utility.Escape;

public class ToolBarLoginAction extends AbstractStructAction {

	@Override
	protected ActionForward beforeLogin(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		Integer hostid = null;

		hostid = PassportManager.getInstance().getHostIdFromCookie(request,
				response, PassportManager.loginKey_ticket);

		String origURL = request.getParameter("origURL");

		StringBuffer srcUrl = request.getRequestURL();
		String origQuery = request.getQueryString();
		srcUrl.append("?");
		srcUrl.append(origQuery);

		request.setAttribute("origURL", srcUrl);
		String errCodeStr = request.getParameter("errCode");

		if (hostid != null) {
			User user = WUserAdapter.getInstance().get(hostid);
			if (user != null) {
				setNextStep(request, false);
				response.sendRedirect(origURL);
				return null;
			} else {
				return fail(mapping, form, request, response,
						LoginStatusCode.FAILNOPOST.getCode());
			}
		} else {

			int passportErrCode = 0;
			try {
				passportErrCode = Integer.parseInt(errCodeStr);
			} catch (NumberFormatException e) {

			}

			if (LoginStatusCode.FAILNOPOST.getCode() == passportErrCode) {
				return mapping.findForward("fail");
			} else {
				return fail(mapping, form, request, response, passportErrCode);
			}

		}

	}

	@Override
	protected ActionForward afterLogin(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		// 可以顺利登录成功后还会涉及用户的帐号是否存在安全问题的部分
		// 此逻辑的具体代码之后会在此处体现
		Integer user_id = null;
		User user = BaseThreadUtil.currentHost(request);
		if (user == null) {
			user_id = (Integer) request.getAttribute(LoginAction.USER_ID);
			if (user_id != null) {
				try {
					user = WUserAdapter.getInstance().get(user_id);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		if (user != null) {
			Runnable flyerTask = new FlyerTaskSet(user.getId(), BizFilterUtil
					.getClientIP(request), request
					.getParameter(LoginParamDef.PARAM_DOMAIN));
			executor.submit(flyerTask);
		}
		return null;
	}

	@Override
	protected ActionForward fail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response,
			int failCode) throws Exception {
		String fail = "fail";
		if (failCode == LoginStatusCode.FAILBANIP.getCode()) {
			// IP被封禁可能会有独立页面显示的需求
			request.setAttribute("error", "您的IP处于被禁用状态");
		} else if (failCode == LoginStatusCode.FAILINPUT.getCode()) {
			request.setAttribute("error", "登录账号或密码错误，请重试");
		} else if (failCode == LoginStatusCode.FAILNOPOST.getCode()) {
			// 以Get方式获取
			request.setAttribute("error", "为保护用户信息需要登录才能执行此操作，请先登录");
		} else if (failCode == LoginStatusCode.FAILUSER.getCode()) {
			// 原代码中此处是从资源文件获取该信息的
			request.setAttribute("error", "登录账号或密码错误，请重试");
		} else if (failCode == LoginStatusCode.FAILUSERACTIVE.getCode()) {
			final String email = Escape.trim(request.getParameter(PARAM_EMAIL));
			final StringBuffer ermsg = new StringBuffer();
			ermsg.append("帐号尚未激活，激活后才可登录。<a href='" + OpiConstants.urlMain
					+ "/reactivate.do'>重发激活信</a>");
			ermsg.append("或用");
			ermsg.append(email);
			ermsg.append("向<strong>hi@" + OpiConstants.domainMain
					+ "</strong> 发封任意邮件");
			request.setAttribute("error", ermsg.toString());
		} else if (failCode == LoginStatusCode.FAILUSERBAN.getCode()) {
			// 原代码中此处是从资源文件获取该信息的
			request.setAttribute("error", "你的帐号因不当行为被管理员封禁，" + "要申诉或解除封禁请"
					+ "<a href=\"" + OpiConstants.urlSupport
					+ "/GetGuestbookList.do\">" + "给管理员留言" + "</a>");
		} else if (failCode == LoginStatusCode.FAILUSERSUICIDE.getCode()) {
			// 原代码中此处是从资源文件获取该信息的
			request
					.setAttribute("error", "您的账号已被冻结，" + "请先" + "<a href=\""
							+ OpiConstants.urlMain + "/relive.do\">" + "解锁帐号"
							+ "</a>。");
		}
		return mapping.findForward(fail);
	}

	@Override
	protected String getIdentify() {
		return "Login-Web";
	}

}
