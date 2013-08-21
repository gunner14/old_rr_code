package com.xiaonei.login.logic.validator;

import net.paoding.rose.web.Invocation;

import com.xiaonei.login.constants.LoginStatusCode;

public class PpParamValidatorUtil {

	public static boolean isCheckNotOk(Invocation inv) {
		return null != inv.getAttribute(LoginStatusCode.class.getSimpleName())
				&& inv.getAttribute(LoginStatusCode.class.getSimpleName()) instanceof Integer
				&& LoginStatusCode.LOGIN_OK != (LoginStatusCode) inv
						.getAttribute(LoginStatusCode.class.getSimpleName());
	}

	public static LoginStatusCode getLoginStatusOfValidation(Invocation inv) {
		return (LoginStatusCode) inv.getAttribute(LoginStatusCode.class
				.getSimpleName());
	}
}
