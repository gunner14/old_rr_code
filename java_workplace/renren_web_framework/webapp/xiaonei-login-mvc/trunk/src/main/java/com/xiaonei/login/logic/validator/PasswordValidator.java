package com.xiaonei.login.logic.validator;

import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.springframework.validation.Errors;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.annotation.Singleton;

@Singleton
public class PasswordValidator implements PpParamValidator {

	@Override
	public Object validate(Invocation inv, PassportForm pForm, Errors errors) {
		if (StringUtils.isEmpty(pForm.getPassword())) {
			inv.setAttribute(loginStatus, LoginStatusCode.FAILINPUT);
			return false;
		}
		return null;
	}

}
