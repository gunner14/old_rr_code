package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserFav;
import com.xiaonei.xce.userfav.UserFavFactory;


public class WUserFavFactory implements UserFavFactory<UserFav> {

	@Override
	public UserFav create() {
		return new UserFav();
	}

}
