package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;

@Service
public class NodeNoPost extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;
	
	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		// 以Get方式获取
		passportMessage.addWarningMessage(inv.getRequest(), "为保护用户信息需要登录才能执行此操作，请先登录");
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILNOPOST;
	}
}
