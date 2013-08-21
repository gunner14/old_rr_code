package com.xiaonei.passport.web.validate;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.model.PassportForm;

public interface PpParamValidator {

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
	public LoginStatusCode validate(PassportForm pForm);
}
