package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserRight;
import com.xiaonei.xce.userright.UserRightFactory;

public class WUserRightFactory implements UserRightFactory<UserRight>{

	@Override
	public UserRight create() {
		return new UserRight();
	}

}
