package com.xiaonei.passport.web.validate;

import java.util.Arrays;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.ArrayUtils;

import com.xiaonei.passport.ObjectMaker;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.web.model.PassportForm;

public class PassportFormValidator{
	private static PassportFormValidator _instace=new PassportFormValidator();
	private PassportFormValidator(){}
	public static PassportFormValidator getInstance(){
		return _instace;
	}
	public LoginStatusCode validate(HttpServletRequest request, PassportForm passportForm, boolean validateAllParams,ValidatorEnum... valiArray) {
		
		LoginLoggerDef.login.trace("formAnn.validateAllParams():"
				+validateAllParams);
		LoginLoggerDef.login.trace("ArrayUtils.isEmpty(formAnn.params()):"
				+ ArrayUtils.isEmpty(valiArray));
		if (validateAllParams && ArrayUtils.isEmpty(valiArray)) {
			valiArray = ValidatorEnum.values();
		} 
		LoginLoggerDef.login.trace("valiArray:"
				+ Arrays.asList(valiArray).toString());

		for (final ValidatorEnum param : valiArray) {
			LoginLoggerDef.login.trace("param.getValidatorClazz():"
					+ param.getValidatorClazz());
			if (null == param.getValidatorClazz()) {
				continue;
			}
			final PpParamValidator va = (PpParamValidator) ObjectMaker
					.getInstance(param.getValidatorClazz());
			LoginLoggerDef.login.trace("va:" + va);
			if (null == va) {
				continue;
			}
			LoginStatusCode code=va.validate(passportForm);
			LoginLoggerDef.login.trace("va.validate(inv, pForm, errors):%s", code);
			if (LoginStatusCode.LOGIN_OK!= code) {
				return code;
			}
		}
		return LoginStatusCode.LOGIN_OK;
	}
	
}
