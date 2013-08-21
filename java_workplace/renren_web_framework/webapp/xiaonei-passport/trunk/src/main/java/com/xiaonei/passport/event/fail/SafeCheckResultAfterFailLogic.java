package com.xiaonei.passport.event.fail;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILoginAfterLoginFail;
import com.xiaonei.passport.logic.safecheck.SafeCheckResult;
import com.xiaonei.passport.model.LoginResult;
@Service
public class SafeCheckResultAfterFailLogic implements ILoginAfterLoginFail{
	private ILoginConfigure loginConfigure=LoginConfigureFactory.getLoginConfigureInstance();
	private SafeCheckResult safeCheckResult=SafeCheckResult.getInstance();
	

	@Override
	public boolean canProcessLoginStatusCode(LoginStatusCode rt) {
		if (rt != LoginStatusCode.FAILUSER
                &&rt!=LoginStatusCode.FAILUSERNOTEXIST) {
            return false;
        }
		return true;
	}


	@Override
	public void process(LoginResult result, ILoginParameter loginParameter) {
		ILoginContext context=loginParameter.getLogincontext();
		int userId=result.getUserId();
		int sourceType=loginConfigure.getUniqLoginConfigureForInt(loginParameter.getLoginRegistrationId(), LoginConfigureKey.LOGIN_SOURCE_TYPE);
		safeCheckResult.safeCheckLogin(userId, sourceType, loginParameter, context);
	}
}
