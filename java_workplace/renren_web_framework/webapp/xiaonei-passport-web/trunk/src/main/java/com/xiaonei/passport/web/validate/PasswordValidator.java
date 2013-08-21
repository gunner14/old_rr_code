package com.xiaonei.passport.web.validate;



import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.model.PassportForm;


@Service
public class PasswordValidator implements PpParamValidator {

	@Override
	public  LoginStatusCode validate(PassportForm pForm) {
		if (StringUtils.isEmpty(pForm.getPassword())) {
			return LoginStatusCode.FAILINPUT;
		}
		 return LoginStatusCode.LOGIN_OK;
	}

}
