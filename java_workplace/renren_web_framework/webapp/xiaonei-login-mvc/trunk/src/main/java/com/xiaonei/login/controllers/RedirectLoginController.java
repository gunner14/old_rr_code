package com.xiaonei.login.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.controllers.base.LoginController;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.login.logic.afterlogin_chains.node.AccountChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.AdminGroupResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CheckUserStatusResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CrossLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.IPBindResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SafeCheckResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SaveTicketChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.TaskAfterLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.WeakPasswordChainNode;
import com.xiaonei.login.logic.failswitcher.ILoginFailSwitch;
import com.xiaonei.login.logic.failswitcher.nodes.rl.NodeDefaultRL;

@Path("RL.do")
@IgnorePassportValidation
public class RedirectLoginController extends LoginController {

	@Autowired
	private SafeCheckResultChainNode safeCheckResultChainNode;
	@Autowired
	private AccountChainNode accountChainNode;
	@Autowired
	private CheckUserStatusResultChainNode checkUserStatusResultChainNode;
	@Autowired
	private IPBindResultChainNode iPBindResultChainNode;
	@Autowired
	private AdminGroupResultChainNode adminGroupResultChainNode;
	@Autowired
	private TaskAfterLoginChainNode taskAfterLoginChainNode;
	@Autowired
	private CrossLoginChainNode crossLoginChainNode;
	@Autowired
	private WeakPasswordChainNode weakPasswordChainNode;
	@Autowired
	private SaveTicketChainNode saveTicketChainNode;
	@Autowired
	private NodeDefaultRL nodeDefaultRL;

	@Override
	protected void ahead(HttpServletRequest request,
			HttpServletResponse response) {
		final String loginType = request.getParameter("loginType");
		if (loginType != null) {
			// 保留登录前缀
			request.setAttribute(LoginParamDef.ATTRIBUTE_LOGINTYPE_PRE,
					loginType);
		}
		String userAgent = request.getHeader("User-Agent");
		if (StringUtils.startsWith(userAgent, "RenRenClient")) {
			request.setAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE, LoginSourceType.Client);
		}else{
			request.setAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE, LoginSourceType.Platform);
		}
		
		if (request.getParameter("p3p") != null) {
			response.setHeader("P3P", "CP=\"CAO PSA OUR\"");
		}
	}

	@Override
	protected boolean needNotify() {
		return false;
	}

	@Override
	protected void setUpFailSwitcher(ILoginFailSwitch failChains) {
		failChains.setUpSwitchNode(nodeDefaultRL);
	}

	@Override
	protected void setUpResultChains(LoginResultChains resultChains) {
		resultChains.setNode(safeCheckResultChainNode);
		resultChains.setNode(accountChainNode);
		resultChains.setNode(checkUserStatusResultChainNode);
		resultChains.setNode(iPBindResultChainNode);
		resultChains.setNode(adminGroupResultChainNode);
		resultChains.setNode(taskAfterLoginChainNode);
		resultChains.setNode(crossLoginChainNode);
		resultChains.setNode(weakPasswordChainNode);
		resultChains.setNode(saveTicketChainNode);
	}
}
