package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.PassportLoginImpl;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;

@Service
@Scope("prototype")
public class TaskAfterLoginChainNode extends AbsLoginResultChainNode {

	@Autowired private PassportLoginImpl passportLogin ;

	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		passportLogin.doTaskAfterLogin(loginModel.getRequest(), loginModel
				.getAccount().getId());
		return next.doChain(loginModel, perCode);
	}

}
