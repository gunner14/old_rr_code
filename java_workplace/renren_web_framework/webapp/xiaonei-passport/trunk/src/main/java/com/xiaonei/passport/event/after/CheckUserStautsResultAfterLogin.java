package com.xiaonei.passport.event.after;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILogicAfterLogin;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.xce.passport.Account;
@Service
public class CheckUserStautsResultAfterLogin implements ILogicAfterLogin{
	
	@Override
	public LoginStatusCode process(LoginResult result, ILoginParameter loginParameter) {
		//把account放到作用域内，外部仅仅只是声明
		Account account;
		switch (result.getStatus()) {
		case User.statusActivateVerified:
			account=new com.xiaonei.xce.passport.Account(result.getUserId(), 0, result.getAccount(), result.getDomain(), result.getStatus(), result.getPassportTicket());
			if (!UserConfirmLogic.getInstance().isConfirmed(account)) {
				return LoginStatusCode.LOGIN_OK;
			}
			return LoginStatusCode.FAILUSERACTIVE;
		case User.statusActivateVerify:
			account=new com.xiaonei.xce.passport.Account(result.getUserId(), 0, result.getAccount(), result.getDomain(), result.getStatus(), result.getPassportTicket());
			if (!UserConfirmLogic.getInstance().isConfirmed(account)) {
				return LoginStatusCode.LOGIN_OK;
			}
			return LoginStatusCode.FAILUSERACTIVE;
		case User.statusBanned:
			return LoginStatusCode.FAILUSERBAN;
		case User.statusSuicide:
			return LoginStatusCode.FAILUSERSUICIDE;
		default:
			return LoginStatusCode.LOGIN_OK;
		}
	}

	
	
}
