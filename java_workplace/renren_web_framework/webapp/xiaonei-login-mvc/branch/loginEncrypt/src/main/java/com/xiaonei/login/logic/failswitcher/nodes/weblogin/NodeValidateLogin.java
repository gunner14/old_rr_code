package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import net.paoding.rose.web.Invocation;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.LoginStatusOprImpl;
import com.xiaonei.login.logic.status.PassportMessageImpl;

@Service
public class NodeValidateLogin extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;
	
	@Autowired
	private LoginStatusOprImpl loginStatusOprImpl;

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		passportMessage.addErrorMessage(inv.getRequest(), loginStatusOprImpl
				.getErrorMessage(LoginStatusCode.FAILVALIDATELOGIN));
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILVALIDATELOGIN;
	}
}
