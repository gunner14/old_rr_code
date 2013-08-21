package com.xiaonei.login.logic.validator;

import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.validation.Errors;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.platform.core.utility.RegularExpression;

@Singleton
public class EmailValidator implements PpParamValidator {

	protected Log log = LogFactory.getLog(this.getClass());

	@Override
	public Object validate(Invocation inv, PassportForm pForm, Errors errors) {

		if (log.isDebugEnabled()) {
			log.debug("login parameters in emailValidate are: "
					+ pForm.toString());
		}
		if (StringUtils.isEmpty(pForm.getEmail())) {
			inv.setAttribute(loginStatus, LoginStatusCode.FAILINPUT);
			return false;
		}
		
		// 长度限制为3~50
		int length = pForm.getEmail().getBytes().length;
		if (length < 3 || length > 50) {
			inv.setAttribute(loginStatus, LoginStatusCode.FAILINPUT);
			return false;
		}

		String email = StringUtils.trimToEmpty(pForm.getEmail());
		email = RegularExpression.processForLogin(email);
		email = email.toLowerCase();
		pForm.setEmail(email);
		return null;
	}
}
