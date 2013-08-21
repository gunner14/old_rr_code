package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.logic.status.ValidationImpl;
import com.xiaonei.login.model.LoginModel;

@Service
@Scope("prototype")
public class CheckUserStatusResultChainNode extends AbsLoginResultChainNode {

	@Autowired private ValidationImpl validation;

	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		// 对当前登录用户状态检验
		final LoginStatusCode status = validation.checkUserStatus(loginModel
				.getAccount());
		if (status != LoginStatusCode.LOGIN_OK) {
			return status;
		}
		return next.doChain(loginModel, perCode);
	}

}
