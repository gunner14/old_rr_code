package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;

import net.paoding.rose.web.Invocation;

@Service
public class NodeNoSafeUser extends AbsLoginFailSwitchNode {

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		// 存在安全问题则跳转至安全中心处理
		return "r:" + "http://safe." + domain + "/home";
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILNOSAFEUSER;
	}
}
