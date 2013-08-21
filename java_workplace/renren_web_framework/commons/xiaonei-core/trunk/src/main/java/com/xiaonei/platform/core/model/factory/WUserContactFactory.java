package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserContact;
import com.xiaonei.xce.usercontact.UserContactFactory;

public class WUserContactFactory implements UserContactFactory<UserContact>{

	@Override
	public UserContact create() {
		return new UserContact();
	}

}
