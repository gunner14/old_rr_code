package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.platform.core.opt.OpiConstants;

import com.xiaonei.platform.core.utility.Escape;

import net.paoding.rose.web.Invocation;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;

@Service
public class NodeUserActive extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;
	
	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		final String email = Escape.trim(inv.getRequest().getParameter(
				LoginParamDef.PARAM_EMAIL));
		final String ermsg = String
				.format(
						"帐号尚未激活，激活后才可登录。<a href='%s/reactivate.do'>重发激活信</a>或用%s向<strong>hi@%s</strong> 发封任意邮件",
						OpiConstants.urlReg.toString(), email,
						OpiConstants.domainMain.toString());
		passportMessage.addErrorMessage(inv.getRequest(), ermsg);
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILUSERACTIVE;
	}
}
