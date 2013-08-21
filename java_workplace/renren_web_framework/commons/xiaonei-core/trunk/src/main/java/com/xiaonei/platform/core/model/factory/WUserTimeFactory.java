package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.xce.usertime.UserTimeFactory;

public class WUserTimeFactory implements UserTimeFactory<UserTime> {

	@Override
	public UserTime create() {
		return new UserTime();
	}

}
