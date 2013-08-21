package com.xiaonei.passport.event.after;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILogicAfterLogin;
import com.xiaonei.passport.logic.adim_groupmember.IAdminGroupmember;
import com.xiaonei.passport.model.LoginResult;
@Service
public class AdminGroupResultAfterLogin implements ILogicAfterLogin{
	@Autowired
	private IAdminGroupmember adminGroup;
	protected Log logger = LogFactory.getLog(this.getClass());
	@Override
	public LoginStatusCode process(LoginResult result,
			ILoginParameter loginParameter) {
		String ip = loginParameter.getLogincontext().getIp();
		int id=result.getUserId();
		// LINKAI需求
		if (logger.isDebugEnabled()) {
			logger.debug(adminGroup.isCouple(id, ip));
		}
		if (adminGroup.isCouple(id, ip)) {
			result.setGotoSafeCenter(true);
		} else if (adminGroup.isDefinedUserId(id)) {
			return LoginStatusCode.FAILNOSAFEUSER;
		}
		return LoginStatusCode.LOGIN_OK;
	}
}
