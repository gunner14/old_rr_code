package com.renren.passport.service;

import java.util.List;
import java.util.Map;

import com.renren.passport.service.ILoginXoaConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;

public class LoginXoaConfigureImpl implements ILoginXoaConfigure{
	private com.xiaonei.passport.configure.ILoginConfigure loginConfigure =LoginConfigureFactory.getLoginConfigureInstance();

	@Override
	public Map<String, List<String>> findLoginConfigure() {
		return loginConfigure.findLoginConfigure();
	}
	
}
