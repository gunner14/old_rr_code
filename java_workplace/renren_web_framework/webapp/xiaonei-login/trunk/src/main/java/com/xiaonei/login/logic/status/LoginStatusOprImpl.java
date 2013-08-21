package com.xiaonei.login.logic.status;

import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.platform.core.opt.OpiConstants;

@Service
public class LoginStatusOprImpl implements LoginStatusOpr {

	@Override
	public String getErrorMessage(LoginStatusCode status) {
		if (status == LoginStatusCode.FAILINPUT) {
			return OpiConstants
					.getDomainIdentifiedProperty("errors.action.login.postneed");
		} else if (status == LoginStatusCode.FAILBANIP) {
			return "你的访问IP已经被封禁";
		} else if (status == LoginStatusCode.FAILUSER) {
			return OpiConstants
					.getDomainIdentifiedProperty("errors.action.login.failed");
		} else if (status == LoginStatusCode.FAILUSERBAN) {
			return String
					.format(
							OpiConstants
									.getDomainIdentifiedProperty("messages.action.login.banned"),
							OpiConstants.urlSupport);
		} else if (status == LoginStatusCode.FAILUSERACTIVE) {
			return OpiConstants
					.getDomainIdentifiedProperty("messages.action.login.notConfirmed");
		} else if (status == LoginStatusCode.FAILUSERSUICIDE) {
			return String
					.format(
							OpiConstants
									.getDomainIdentifiedProperty("messages.action.login.suicided"),
							OpiConstants.urlMain);
		} else if (status == LoginStatusCode.FAILVALIDATELOGIN) {
			return "验证码输入错误";
		} else if (status == LoginStatusCode.FAILNOTIFYGRAYUSER) {
			return "帐号出现异常，为了您的帐号安全，请尽快修改登录密码！";
		} else if (status == LoginStatusCode.FAILNOTIFYGRAYIP) {
			return "帐号出现异常，为了您的帐号安全，请尽快修改登录密码！";
		}
		return "";
	}

}
