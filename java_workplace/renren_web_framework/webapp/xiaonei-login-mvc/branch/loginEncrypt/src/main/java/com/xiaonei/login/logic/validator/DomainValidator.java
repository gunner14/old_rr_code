package com.xiaonei.login.logic.validator;

import net.paoding.rose.web.Invocation;

import org.springframework.validation.Errors;

import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.annotation.Singleton;

@Singleton
public class DomainValidator implements PpParamValidator {

	@Override
	public Object validate(Invocation inv, PassportForm pForm, Errors errors) {
		pForm.setDomain(LoginLogicFactory.getDomainOpr().getDomainParam(
				pForm.getDomain(), inv.getRequest()));
		return null;
	}

}
