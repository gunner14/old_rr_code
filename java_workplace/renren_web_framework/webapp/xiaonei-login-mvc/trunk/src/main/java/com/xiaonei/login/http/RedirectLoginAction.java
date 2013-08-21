package com.xiaonei.login.http;

import java.net.URLEncoder;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.utility.DES_Encrypt;
import com.xiaonei.platform.core.utility.Escape;

public class RedirectLoginAction extends WebLoginAction {

	@Override
	protected boolean needNotify() {
		return super.needNotify();
	}

	public RedirectLoginAction() {
		super();
	}

	@Override
	protected ActionForward beforeLogin(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		if (request.getParameter("p3p") != null) {
			response.setHeader("P3P", "CP=\"CAO PSA OUR\"");
		}
		// 从Cookie验票过程
		ActionForward forward = getRequest(mapping, form, request, response);
		if (forward != null) {
			return forward;
		}

		setUserName(request, request.getParameter(PARAM_EMAIL));
		setPassword(request, request.getParameter(PARAM_PASSWORD));

		// 记住用户名，用于下次在输入框自动显示
		String userName = Escape.trim(request.getParameter(PARAM_EMAIL));
		if (userName != null && userName.length() > 0) {
			Cookie deUserName = new Cookie("_de", DES_Encrypt.encrypt(userName));
			deUserName.setDomain("." + OpiConstants.domain);
			deUserName.setMaxAge(60 * 60 * 24 * 30 * 12);// one year
			response.addCookie(deUserName);
		}
		// 验证密码前还会涉及用户帐号安全登录的部分
		// 此逻辑的具体代码之后会在此处体现

		return null;
	}

	@Override
	protected ActionForward fail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response,
			int failCode) throws Exception {
		String domain = (String) request.getAttribute(PARAM_DOMAIN);
		LoginLogicFactory.getDomainOpr().setDomainEnv(request);
		String errorMsg = null;
		if (failCode == LoginStatusCode.FAILBANIP.getCode()) {
			// IP被封禁可能会有独立页面显示的需求
			errorMsg = "您的IP处于被禁用状态";
		} else if (failCode == LoginStatusCode.FAILUSERNOTEXIST.getCode()) {
			// if (OpiConstants.isKaixin()) {
			// //开心用户不存在失败逻辑
			// }
			// 其余逻辑与输入错误一样
			errorMsg = "登录帐号或密码错误，请重试";
		} else if (failCode == LoginStatusCode.FAILINPUT.getCode()) {
			errorMsg = "登录帐号或密码错误，请重试";
		} else if (failCode == LoginStatusCode.FAILNOPOST.getCode()) {
			// 以Get方式获取
			errorMsg = "为保护用户信息需要登录才能执行此操作，请先登录";
		} else if (failCode == LoginStatusCode.FAILUSER.getCode()) {
			// 原代码中此处是从资源文件获取该信息的
			errorMsg = "登录帐号或密码错误，请重试";
		} else if (failCode == LoginStatusCode.FAILUSERACTIVE.getCode()) {
			String email = Escape.trim(request.getParameter(PARAM_EMAIL));
			StringBuffer ermsg = new StringBuffer();
			ermsg.append("帐号尚未激活，激活后才可登录。<a href='" + "http://" + domain
					+ "/reactivate.do'>重发激活信</a>");
			ermsg.append("或用");
			ermsg.append(email);
			ermsg.append("向<strong>hi@" + OpiConstants.domainMain
					+ "</strong> 发封任意邮件");
			errorMsg = ermsg.toString();
		} else if (failCode == LoginStatusCode.FAILUSERBAN.getCode()) {
			// 原代码中此处是从资源文件获取该信息的
			errorMsg = "你的帐号因不当行为被管理员封禁，" + "要申诉或解除封禁请" + "<a href=\""
					+ "http://support." + domain + "/GetGuestbookList.do\">"
					+ "给管理员留言" + "</a>";
		} else if (failCode == LoginStatusCode.FAILUSERSUICIDE.getCode()) {
			// 原代码中此处是从资源文件获取该信息的
			errorMsg = "您的帐号已被冻结，" + "请先" + "<a href=\"" + "http://" + domain
					+ "/relive.do\">" + "解锁帐号" + "</a>。";
		} else if (failCode == LoginStatusCode.FAILNOSAFEUSER.getCode()) {
			// 存在安全问题则跳转至安全中心处理
			failCode = LoginStatusCode.FAILUNKNOWN.getCode();
		} else {
			failCode = LoginStatusCode.FAILUNKNOWN.getCode();
		}
		String origUrl = getRedirectUrl(request);
		request.setAttribute(PARAM_DOMAIN, request.getParameter(PARAM_DOMAIN));
		if (errorMsg != null) {
			errorMsg = URLEncoder.encode(errorMsg, "UTF-8");
		}
		if (origUrl.endsWith("/SysHome.do")) {

		} else if (origUrl.indexOf("?") > 0) {
			origUrl += "&errCode=" + failCode;
		} else {
			origUrl += "?errCode=" + failCode;
		}
		return new ActionForward(origUrl, true);
	}

	@Override
	public String getActionName() {
		return "RL.do";
	}
}
