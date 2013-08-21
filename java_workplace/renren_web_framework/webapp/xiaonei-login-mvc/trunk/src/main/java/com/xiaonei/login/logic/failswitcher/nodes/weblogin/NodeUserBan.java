package com.xiaonei.login.logic.failswitcher.nodes.weblogin;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.login.logic.status.PassportMessageImpl;
import com.xiaonei.platform.core.opt.OpiConstants;
import net.paoding.rose.web.Invocation;

@Service
public class NodeUserBan extends AbsLoginFailSwitchNode {

	@Autowired
	private PassportMessageImpl passportMessage;

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		passportMessage.addErrorMessage(inv.getRequest(), String.format(
				"你的帐号已停止使用，需要申诉请<a href=\"%s/GetGuestbookList.do\">给管理员留言</a>",
				OpiConstants.urlSupport.toString()));
		return super.getFailUri(inv);
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILUSERBAN;
	}
}
