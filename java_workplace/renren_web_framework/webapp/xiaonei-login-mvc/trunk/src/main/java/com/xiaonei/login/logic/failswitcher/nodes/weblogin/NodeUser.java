package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.autologin.AutoLoginEmailLogicImpl;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.login.logic.email.PassportEmailUtilsImpl;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.core.utility.StringFormat;

@Service
public class NodeUser extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;

	@Autowired
	private AutoLoginEmailLogicImpl autoLoginEmailLogicImpl;

	@Autowired
	private PassportEmailUtilsImpl passportEmailUtilsImpl;

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		final String email = Escape.trim(inv.getRequest().getParameter(
				LoginParamDef.PARAM_EMAIL));
		int loginCount=CaptchaAdapterFactory.getCaptchaAdapterInstance().getLoginCountFromRequest(inv.getRequest());
		if (loginCount < 5 || !StringFormat.isValidEmailAddr(email)
				|| StringUtils.endsWith(email, "@qq.com")) {
			passportMessage.addErrorMessage(inv.getRequest(), "登录帐号或密码错误，请重试");
		}  else  {
			String emailLoginUrl = passportEmailUtilsImpl.emailLoginUrl(email);
			emailLoginUrl = StringUtils.isEmpty(emailLoginUrl) ? "#"
					: emailLoginUrl;
			inv.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_EMAIL_LOGIN_URL, emailLoginUrl);
			passportMessage
					.addErrorMessage(
							inv.getRequest(),
							String
									.format(
											"你的用户名和密码不匹配，我们已经发送了一封确认邮件到<a href='%s'>%s</a>, 请通过点击邮件内链接登录。",
											emailLoginUrl, email));
			autoLoginEmailLogicImpl.sendAutoLoginEmail(email, domain);
		}
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILUSER;
	}
}
