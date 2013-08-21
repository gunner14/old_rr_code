package com.xiaonei.login.logic.failswitcher.nodes.rl;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.DomainOprImpl;
import com.xiaonei.login.logic.failswitcher.nodes.AbsLoginFailSwitchNode;
import com.xiaonei.platform.core.opt.OpiConstants;

@Service
public class NodeDefaultRL extends AbsLoginFailSwitchNode {
	
	@Autowired
	private DomainOprImpl domainOprImpl;

	@Override
	public String fail(String domain, Invocation inv, LoginStatusCode status) {
		domainOprImpl.setDomainEnv(inv.getRequest(), domain);
		String origUrl = getRedirectUrl(inv.getRequest());
		inv.addModel(LoginParamDef.PARAM_DOMAIN, domain);
		if (origUrl.endsWith("/SysHome.do")) {

		} else if (origUrl.indexOf("?") > 0) {
			origUrl += "&errCode=" + status.getCode();
		} else {
			origUrl += "?errCode=" + status.getCode();
		}
		return "r:" + origUrl;
	}

	@Override
	public LoginStatusCode getTag() {
		return LoginStatusCode.FAILDEFAULT;
	}
	
	private String getRedirectUrl(HttpServletRequest request) {
		String homeURL = OpiConstants.urlHome + "/Home.do";
		String origURL = request.getParameter(LoginParamDef.PARAM_ORIGURL);
		origURL = origURL != null && origURL.length() > 0 ? origURL : homeURL;
		if (origURL.contains(OpiConstants.urlLogin + "/GLogin.do")
		// 暂时删了 || origURL.contains("SysHome.do") // 不跳回主页
				|| origURL.contains(OpiConstants.urlPassport + "/PLogin.do")) {
			origURL = homeURL;
		}
		return origURL;
	}

}
