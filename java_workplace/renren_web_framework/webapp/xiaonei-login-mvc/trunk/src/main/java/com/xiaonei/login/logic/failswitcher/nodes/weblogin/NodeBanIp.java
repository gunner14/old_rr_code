package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;


@Service
public class NodeBanIp extends AbsLoginFailSwitchNode {
	
	@Autowired
	private PassportMessageImpl passportMessage;


	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		// IP被封禁可能会有独立页面显示的需求
		passportMessage.addErrorMessage(inv.getRequest(),
				"您的IP处于被禁用状态");
		return super.getFailUri(inv);
	}

	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILBANIP;
	}

}
