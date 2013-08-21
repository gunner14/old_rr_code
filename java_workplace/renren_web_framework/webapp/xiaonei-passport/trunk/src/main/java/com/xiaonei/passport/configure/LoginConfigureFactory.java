package com.xiaonei.passport.configure;

import com.xiaonei.passport.configure.impl.LoginConfigureDaoImpl;
import com.xiaonei.passport.configure.impl.LoginConfigureImpl;

public class LoginConfigureFactory {
	 /**
	 * 
	 * @return ILoginConfigureDao
	 */
	public static ILoginConfigureDao getLoginConfigureDaoInstance(){
		return LoginConfigureDaoImpl.getInstance();
	}
	
	/**
	 * 
	 * @return ILoginConfigure
	 */
	public static ILoginConfigure getLoginConfigureInstance(){
		return LoginConfigureImpl.getInstance();
	}
}
