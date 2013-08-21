package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.xce.username.UserNameFactory;

public class WUserNameFactory implements UserNameFactory<UserName>{

	@Override
	public UserName create() {
		return new UserName();
	}

}
