package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.logic.status.Validation;
import com.xiaonei.login.model.LoginModel;

@Service
@Scope("prototype")
public class IPBindResultChainNode extends AbsLoginResultChainNode {

	@Autowired private Validation validation;

	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {

		if (validation.isBindedIPDoNotBind(loginModel.getIp(), loginModel
				.getAccount().getId())) {
			return LoginStatusCode.FAILBANIP;
		}

		return next.doChain(loginModel, perCode);
	}

}
