package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.userbaseview.UserBaseCommonViewFactory;

public class WUserCommonFactory  implements UserBaseCommonViewFactory<User>{

	@Override
	public User create() {
		return new User();
	}
	
}
