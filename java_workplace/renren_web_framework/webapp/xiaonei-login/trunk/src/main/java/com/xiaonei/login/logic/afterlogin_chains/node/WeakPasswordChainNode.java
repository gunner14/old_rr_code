package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.logic.status.ValidationImpl;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.platform.core.utility.Encoder;

@Service
@Scope("prototype")
public class WeakPasswordChainNode extends AbsLoginResultChainNode {

	@Autowired private ValidationImpl validation;

	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		// 弱密码判断
		final boolean isPasswordEncode = loginModel.getRequest().getAttribute(
				LoginParamDef.PARAM_ENPWD) != null ? true : false;
		final String passwordEncode = isPasswordEncode ? loginModel.getPwd()
				: Encoder.encode(loginModel.getPwd());
		if (validation.isWeekPasswordInMd5Format(passwordEncode)) {
			validation.setWarningTagOfWeakPasswordInAttribut(loginModel
					.getRequest());
		}
		return next.doChain(loginModel, perCode);
	}

}
