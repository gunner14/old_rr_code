package com.xiaonei.passport.web.adapt;

import com.renren.passport.service.ILoginService;

public class PassportAdaptFactory {
	public static ILoginService getLoginService(){
		return WebLoginServiceImpl.getInstance();
	}
	
	public static com.renren.passport.service.ILoginConfigure getLoginConfigure(){
		return LoginConfigureImpl.getInstance();
	}
}
