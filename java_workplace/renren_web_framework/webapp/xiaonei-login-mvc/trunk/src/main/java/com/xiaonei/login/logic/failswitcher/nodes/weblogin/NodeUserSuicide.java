package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;

import net.paoding.rose.web.Invocation;

@Service
public class NodeUserSuicide extends AbsLoginFailSwitchNode {

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		// 有独立的错误信息格式
		inv.getRequest().setAttribute("SERSUICIDE", true);
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILUSERSUICIDE;
	}
}
