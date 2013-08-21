package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.xce.userpassport.UserPassportFactory;

public class WUserPassportFactory implements UserPassportFactory<UserPassport>{

	@Override
	public UserPassport create() {
		return new UserPassport();
	}

}
