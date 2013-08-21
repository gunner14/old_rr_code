package com.xiaonei.login.logic.failforreg;

import static com.xiaonei.login.constants.LoginFailToRegFlowDef.blackEmailList;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;


import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.logic.MemCacheFactory;
import com.xiaonei.login.logic.status.PassportMessageImpl;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.opt.OpiConstants;

@Service
public class LoginFailedForRegistrationProcessImpl implements
		LoginFailedForRegistrationProcess, LoginLoggerDef {

	private MemCacheFactory memCacheFactory = MemCacheFactory.getInstance();
	
	@Autowired
	private PassportMessageImpl passportMessageImpl;

	@Override
	public boolean isLoginFailForReg(String email, HttpServletRequest request) {
		if (!loginFailToRegSwitch.isEnable()) {
			return false;
		}
		if (OpiConstants.isXiaonei()
				&& !"true".equals(request
						.getParameter(LoginParamDef.PARAM_ISPLOGIN))) {
			return false;
		}
		email = StringUtils.trimToEmpty(email);
		final String[] emailParts = StringUtils.split(email, "@");
		if (emailParts == null) {
			return false;
		}
		if (emailParts.length != 2) {
			return false;
		}
		if (!Boolean.valueOf(true).equals(blackEmailList.get(emailParts[1]))) {
			return false;
		}
		return true;
	}

	@Override
	public String loginFailForReg(Invocation inv) {
		// 登录失败逻辑
		try {
			final String sessionid = loginFailForRegSaveUserInfoIntoMemcache(inv);

			// 跳转到注册的公用页面
			if (OpiConstants.isKaixin()) {
				login.info(loginLogPentter, "kaixin login fail for reg", inv
						.getRequest().getParameter("email"), sessionid);
				return "r:" + OpiConstants.urlRegJump
						+ "/kx.do?ss=10122&rt=72&f=" + sessionid;
			} else {
				login.info(loginLogPentter, "renren login fail for reg", inv
						.getRequest().getParameter("email"), sessionid);
				return "r:" + OpiConstants.urlRegJump
						+ "/xn.do?ss=10126&rt=107&fr=" + sessionid;
			}
		} catch (final Exception e) {
			e.printStackTrace();
			addErrorMessage(inv.getRequest(), "登录帐号或密码错误，请重试");
			return null;
		}
	}

	@Override
	public String loginFailForRegSaveUserInfoIntoMemcache(final Invocation inv) {
		// 保存用户输入的东西
		final MemCacheManager mem = memCacheFactory
				.getManagerByPoolName(MemCacheKeys.pool_default);
		final String sessionid = UUID.randomUUID().toString();
		final String key = "loginfail_usernofound_" + sessionid;
		final Map<String, String> params = new HashMap<String, String>();
		for (final Object param : inv.getRequest().getParameterMap().keySet()) {
			params.put(param.toString(), inv.getRequest().getParameter(
					param.toString()));
		}
		mem.set(key, params, 60);
		return sessionid;
	}

	private void addErrorMessage(HttpServletRequest request, String message) {
		passportMessageImpl.addErrorMessage(request, message);
	}

	public LoginFailedForRegistrationProcessImpl bindMemCacheFactory(
			MemCacheFactory memCacheFactory) {
		this.memCacheFactory = memCacheFactory;
		return this;
	}

}
