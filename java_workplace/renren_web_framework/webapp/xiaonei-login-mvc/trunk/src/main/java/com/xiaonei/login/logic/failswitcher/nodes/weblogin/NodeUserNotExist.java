package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import javax.servlet.http.HttpServletRequest;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import net.paoding.rose.web.Invocation;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failforreg.LoginFailedForRegistrationProcessImpl;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;

@Service
public class NodeUserNotExist extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;
	
	@Autowired
	private LoginFailedForRegistrationProcessImpl failedForRegistrationProcessImpl;

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		final String email = StringUtils.trimToEmpty(inv.getRequest()
				.getParameter(LoginParamDef.PARAM_EMAIL));

		if (!isLoginFailForReg(email, inv.getRequest())) {
			passportMessage.addErrorMessage(inv.getRequest(), "登录帐号或密码错误，请重试");
			return super.getFailUri(inv);
		}
		return LoginFailForReg(inv);
	}

	private String LoginFailForReg(Invocation inv) {
		return failedForRegistrationProcessImpl.loginFailForReg(inv);
	}

	private boolean isLoginFailForReg(String email, HttpServletRequest requset) {
		return failedForRegistrationProcessImpl.isLoginFailForReg(email,
				requset);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILUSERNOTEXIST;
	}
}
