package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.admin_groupmember.IAdminGroupmember;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.xce.passport.Account;

@Service
@Scope("prototype")
public class AdminGroupResultChainNode extends AbsLoginResultChainNode {

    @Autowired
	private IAdminGroupmember adminGroup;
	
	public AdminGroupResultChainNode() {
	}

//	public AdminGroupResultChainNode(IAdminGroupmember adminGroup) {
//		this.adminGroup = adminGroup;
//	}
	
//	public void setAdminGroup(IAdminGroupmember adminGroup) {
//		this.adminGroup = adminGroup;
//	}

	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		Account account = loginModel.getAccount();
		String ip = loginModel.getIp();
		// LINKAI需求
		if (logger.isDebugEnabled()) {
			logger.debug(adminGroup.isCouple(account.getId(), ip));
		}
		if (adminGroup.isCouple(account.getId(), ip)) {
			loginModel.getRequest().setAttribute(
					LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING, "lk");
		} else if (adminGroup.isDefinedUserId(account.getId())) {
			return LoginStatusCode.FAILNOSAFEUSER;
		}
		return next.doChain(loginModel, perCode);
	}

}
