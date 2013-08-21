package com.xiaonei.login.logic.validator;

import net.paoding.rose.web.Invocation;

import org.springframework.validation.Errors;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.form.PassportForm;

public interface PpParamValidator {

	String loginStatus = LoginStatusCode.class.getSimpleName();

	/**
	 * validate<br>
	 * 验证PassportForm
	 * 
	 * @param inv
	 * @param target
	 * @param errors
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 15, 2010 - 3:28:04 PM
	 */
	public Object validate(Invocation inv, PassportForm pForm, Errors errors);
}
