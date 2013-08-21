package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;

import net.paoding.rose.web.Invocation;

@Service
public class NodeDefault extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;
	
	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		passportMessage.addWarningMessage(inv.getRequest(), "系统异常,请稍候重试!!!");
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILDEFAULT;
	}

}
