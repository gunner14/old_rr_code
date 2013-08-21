package com.xiaonei.passport.event.before;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILogicBeforeLogin;
/**
 * 如果提交参数不合法，则直接拒掉，所有参数的校验都在这里处理。
 * @author Administrator
 *
 */
@Service
public class ValidateParameterBeforeLogin implements ILogicBeforeLogin{


	@Override
	public LoginStatusCode process(ILoginParameter loginParameter) {
		if(StringUtils.isEmpty(loginParameter.getUserName())){
			return LoginStatusCode.FAILINPUT;
		}
		if(StringUtils.isEmpty(loginParameter.getMd5Password())){
			return LoginStatusCode.FAILINPUT;
		}
		if(loginParameter.getLoginRegistrationId()<1){
			return LoginStatusCode.FAILINPUT;
		}
		ILoginContext context=loginParameter.getLogincontext();
		if(context==null){
			return LoginStatusCode.FAILINPUT;
		}
		return LoginStatusCode.LOGIN_OK;
	}

}
