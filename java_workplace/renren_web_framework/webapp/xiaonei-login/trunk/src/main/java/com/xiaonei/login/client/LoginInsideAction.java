package com.xiaonei.login.client;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.login.client.base.AbstractStructAction;
import com.xiaonei.login.client.base.LoginAction;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.platform.component.application.cache.AppCacheMgr;
import com.xiaonei.platform.component.application.home.AppBasicHome;
import com.xiaonei.platform.component.application.home.AppConnectHome;
import com.xiaonei.platform.component.application.home.AppUserAuthorizedHome;
import com.xiaonei.platform.component.application.model.AppActiveUserLog;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.AppConnect;
import com.xiaonei.platform.component.application.model.AuthToken;
import com.xiaonei.platform.component.application.sk.mgr.SessionKeyManager;
import com.xiaonei.platform.component.application.sk.model.SessionKey;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 登录Action<br>
 * 1、如果没有传入api_key:登录页只有email和pass域。登录后跳到 xiaonei home页<br>
 * 2、如果传入的api_key正确，v参数没有传入或传入错误:登录页有email、pass域，还有app title、安全Note。登录后跳到
 * website站点<br>
 * 3、如果传入的api_key正确，v参数传入且正确:登录页有email、pass域，还有app title、checkbox、安全Note。登录后跳到
 * website站点<br>
 * 4、如果api_key传入，但值错误:登录页有email和pass域，还有error消息。登录后跳到 xiaonei home页<br>
 * LAction
 * 
 * @author lifs
 */
public class LoginInsideAction extends AbstractStructAction {

	public static final String PASSPORT_LOGIN_URL = "http://passport.renren.com/RL.do";

	@SuppressWarnings({ "unchecked", "deprecation" })
	@Override
	public ActionForward beforeLogin(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException, UnsupportedEncodingException {

		String ak = AntiSpamAdapter.getInstance().checkAndFilter(0,
				CheckType.PRUE, request.getParameter("api_key"))
				.getSafePureContent();// apiKey
		String ver = AntiSpamAdapter.getInstance().checkAndFilter(0,
				CheckType.PRUE, request.getParameter("v")).getSafePureContent();
		// 版本 v=1.0
		String authToken = AntiSpamAdapter.getInstance().checkAndFilter(0,
				CheckType.PRUE, request.getParameter("auth_token"))
				.getSafePureContent();
		if (StringUtils.isEmpty(ak)) {
			addErrorMessage(request, "没有传入指定的api_key参数");
			this.setNextStep(request, false);
			return mapping.findForward("fail");
		}
		AppBasic ab = AppBasicHome.getInstance().getByApiKey(ak);
		if (ab == null) {
			addErrorMessage(request, "传入无效的api_key参数");
			this.setNextStep(request, false);
			return mapping.findForward("fail");
		}
		String context = AntiSpamAdapter.getInstance().checkAndFilter(0,
				CheckType.PRUE, request.getParameter("context"))
				.getSafePureContent();
		AppMenuItem am = AppMenuItemCacheMgr.getInstance().get(ab.getAppId());
		AppConnect ac = AppConnectHome.getInstance().getAppConnect(
				ab.getAppId());
		AuthToken au = AppCacheMgr.getAuthToken(authToken);

		request.setAttribute("ab", ab);
		request.setAttribute("appId", am.getAppId());
		request.setAttribute("appName", am.getAppName());
		request.setAttribute("appLogo", ac.getConnect_logo());
		// request.setAttribute("appCallBackUrl", ab.getCallbackUrl());
		request.setAttribute("au", au);// 将au放入request中
		request.setAttribute("api_key", ak);
		request.setAttribute("api_version", ver);
		request.setAttribute("auth_token", authToken);
		request.setAttribute("context", context);
		// 设置回调passport 回调地址
		request.setAttribute("passportLoginUrl", PASSPORT_LOGIN_URL);
		// StringBuffer origUrl = request.getRequestURL();
		// String origQuery = request.getQueryString();
		// origUrl.append("?");
		// origUrl.append(origQuery);
		StringBuffer origUrl = request.getRequestURL();
		Map<String, String[]> paramMap = (Map<String, String[]>) request
				.getParameterMap();
		StringBuffer query = new StringBuffer(512);
		boolean firstParam = true;
		for (Entry<String, String[]> entry : paramMap.entrySet()) {
			if (!"errCode".equals(entry.getKey())) {
				if (!firstParam) {
					query.append("&");
				} else {
					firstParam = false;
				}
				query.append(entry.getKey() + "="
						+ URLEncoder.encode(entry.getValue()[0], "UTF-8"));
			}
		}
		origUrl.append("?");
		origUrl.append(query);
		request.setAttribute("origURL", origUrl);

		if (au != null && au.getAppId() != am.getAppId()) {
			addErrorMessage(request, "传入无效的api_key参数");
			this.setNextStep(request, false);
			return mapping.findForward("fail");
		}
		if (StringUtils.isNotEmpty(authToken) && au == null) {
			addErrorMessage(request, "本次登录已经过期失效，请返回 :)");
			this.setNextStep(request, false);
			return mapping.findForward("fail");
		}

		int passportErrCode = 0;
		try {
			passportErrCode = Integer.parseInt(request.getParameter("errCode"));
		} catch (NumberFormatException e) {
			// Do nothing
		}

		// 这段 post逻辑可能
		// if (!"post".equalsIgnoreCase(request.getMethod())) {
		// setNextStep(request, false);
		// return mapping.findForward("fail");
		// }

		if (passportErrCode == 0) {
			Integer hostid = ApiLoginUtil.validatePassportTicket(request,
					response);
			if (hostid != null) { // 验票成功，得到这个票是什么（只是验证T票）
				User host = WUserAdapter.getInstance().get(hostid);
				if (host != null) {
					BaseThreadUtil.setHost(request, host);
					setNextStep(request, false);
					request.setAttribute(LoginAction.USER_ID, hostid);
					return afterLogin(mapping, form, request, response);
				}

			} else {
				setNextStep(request, false);
				return mapping.findForward("fail");
			}

		} else {
			setNextStep(request, false);
			return fail(mapping, form, request, response, passportErrCode);
		}

		// 修改后 后面的逻辑已经不会执行到
		// this.setUserName(request,
		// Escape.trim(request.getParameter("email")));
		// this.setPassword(request,
		// Escape.trim(request.getParameter("password")));
		setNextStep(request, false);
		return mapping.findForward("fail");
	}

	/**
	 * 用户是否已经授权过了此应用
	 * 
	 * @param appId
	 * @param userId
	 * @return 返回true 用户已经授权过此应用了
	 */
	@SuppressWarnings("unused")
	private boolean isAuthorizedApp(int appId, int userId) {
		// 是否已经是此application的用户
		if (AppUserAuthorizedHome.getInstance().get(appId, userId) == null) {
			return false;
		} else {
			return true;
		}
	}

	@SuppressWarnings({ "static-access", "deprecation" })
	@Override
	protected ActionForward afterLogin(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		// 没有api_key 或 api_key不是系统的 ，到home页面
		// 有api_key 且 api_key 是系统的-->
		// 用户如果不是application的用户，到tos页面；用户如果是app的用户，则生成token和跳转js，进入到callbackurl
		String authToken = AntiSpamAdapter.getInstance().checkAndFilter(0,
				CheckType.PRUE, request.getParameter("auth_token"))
				.getSafePureContent();
		// User host = BaseThreadUtil.currentHost(request);
		Object object = request.getAttribute(LoginAction.USER_ID);

		Integer userId = null;
		try {
			userId = (Integer) object;
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (userId == null) {
			return fail(mapping, form, request, response,
					ApiLoginErrorCode.LOGIN_BUSY);
		}

		AppBasic ab = (AppBasic) request.getAttribute("ab");// 强制获取
		String context = AntiSpamAdapter.getInstance().checkAndFilter(0,
				CheckType.PRUE, request.getParameter("context"))
				.getSafePureContent();
		// 用户是否已经授权过了此应用
		// if(isAuthorizedApp(ab.getAppId(), host.getId())){
		// 生成token、跳转到Js流
		request.setAttribute("callback_url", ab.getCallbackUrl());

		String token = authToken;
		SessionKey sk = SessionKeyManager.getInstance().getSessionKey(
				SessionKeyManager.SESSION_KEY_CANVASDESKTOP, userId,
				ab.getAppId(), true);
		if (StringUtils.isNotEmpty(authToken)) {
			// 此处将sessionKey放入authToken对象中供getSession接口调用
			AuthToken au = (AuthToken) request.getAttribute("au");// 获取上层传入的authToken
			if (au != null) {
				au.setSessionKey(sk.getSessionKey());
				au.setSessionSecret(sk.getSessionSecret());
				au.setUserId(userId);
				AppCacheMgr.setAuthToken(authToken, au);
			}
		} else {
			// 处理不是web类型的应用
			token = UUID.randomUUID().toString().replaceAll("-", "");
			AuthToken at = new AuthToken();
			at.setAppId(ab.getAppId());
			at.setToken(token);
			at.setSessionKey(sk.getSessionKey());
			at.setUserId(userId);
			at.setSessionSecret(sk.getSessionSecret());
			AppCacheMgr.setAuthToken(token, at);
		}
		List<String> paramList = new ArrayList<String>();
		paramList.add("auth_token=" + token);
		paramList.add("uid=" + userId);
		ApiLoginUtil.updateActiveUserStat(userId, ab.getAppId(),
				AppActiveUserLog.TYPE_WWW, request, response);// 加入统计信息
		this.setNextStep(request, false);
		if (StringUtils.isNotBlank(context)) {// 支持传入context参数的逻辑
			paramList.add("context=" + context);
		}
		String signature = this.generateSignature(paramList, ab.getSecret());
		StringBuffer stringBuffer = new StringBuffer();
		for (String a : paramList) {
			stringBuffer.append(a);
			stringBuffer.append("&");
		}
		stringBuffer.append("sig=");
		stringBuffer.append(signature);
		request.setAttribute("auth_token", stringBuffer.toString());
		return mapping.findForward("token");
	}

	@Override
	protected ActionForward fail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response,
			int failCode) {

		if (failCode == LoginStatusCode.FAILINPUT.getCode()) {// 账号、密码 输入验证失败
			addErrorMessage(request, "用户输入不合法");
		} else if (failCode == LoginStatusCode.FAILBANIP.getCode()) {// 地址封禁过滤
			addErrorMessage(request, "用户当前登录IP被封禁");
		} else if (failCode == LoginStatusCode.FAILUSER.getCode()) {// 没有此用户
			addErrorMessage(request, "用户输入的用户名或者密码错误");
		} else if (failCode == LoginStatusCode.FAILUSERACTIVE.getCode()) {// 帐号未激活
			addErrorMessage(request, "用户帐号未激活");
		} else if (failCode == LoginStatusCode.FAILUSERBAN.getCode()) {// 帐号被封禁
			addErrorMessage(request, "用户帐号被封禁");
		} else if (failCode == LoginStatusCode.FAILUSERSUICIDE.getCode()) {// 帐号已经注销
			addErrorMessage(request, "帐号已经注销");
		} else if (failCode == LoginStatusCode.FAILUSERNOTEXIST.getCode()) {// 帐号不存在
			addErrorMessage(request, "用户帐号不存在");
		} else if (failCode == LoginStatusCode.FAILNOSAFEUSER.getCode()) {// 帐号不存在
			addErrorMessage(request, "用户帐号存在安全问题");
		} else if (failCode == 0) {
			addErrorMessage(request, "登录失败");
		}
		return mapping.findForward("fail");
	}

	@Override
	protected String getIdentify() {
		return null;
	}

	/**
	 * 用户生成md5加密的方法
	 * 
	 * @param params
	 * @param secret
	 * @return
	 */
	public static String generateSignature(List<String> params, String secret) {
		StringBuffer buffer = new StringBuffer();
		Collections.sort(params);
		for (String param : params) {
			buffer.append(param);
		}

		buffer.append(secret);
		try {
			java.security.MessageDigest md = java.security.MessageDigest
					.getInstance("MD5");
			StringBuffer result = new StringBuffer();
			try {
				for (byte b : md.digest(buffer.toString().getBytes("UTF-8"))) {
					result.append(Integer.toHexString((b & 0xf0) >>> 4));
					result.append(Integer.toHexString(b & 0x0f));
				}
			} catch (UnsupportedEncodingException e) {
				for (byte b : md.digest(buffer.toString().getBytes())) {
					result.append(Integer.toHexString((b & 0xf0) >>> 4));
					result.append(Integer.toHexString(b & 0x0f));
				}
			}
			return result.toString();
		} catch (java.security.NoSuchAlgorithmException ex) {
			System.err.println("MD5 does not appear to be supported" + ex);
			return "";
		}
	}
}
