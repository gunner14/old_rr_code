package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserUrl;
import com.xiaonei.xce.userurl.UserUrlFactory;

public class WUserUrlFactory implements UserUrlFactory<UserUrl> {

	@Override
	public UserUrl create() {
		return new UserUrl();
	}

}
