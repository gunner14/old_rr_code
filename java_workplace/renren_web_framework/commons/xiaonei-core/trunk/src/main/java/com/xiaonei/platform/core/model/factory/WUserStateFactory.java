package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.xce.userstate.UserStateFactory;

public class WUserStateFactory implements UserStateFactory<UserState>{

	@Override
	public UserState create() {
		return new UserState();
	}
	
}
