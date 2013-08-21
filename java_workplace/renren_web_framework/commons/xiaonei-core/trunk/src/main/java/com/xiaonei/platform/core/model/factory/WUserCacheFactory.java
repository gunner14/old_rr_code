package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.xce.usercache.UserCacheFactory;


public class WUserCacheFactory implements UserCacheFactory<WUserCache>{
	public WUserCache createUserCache() {
		return new WUserCache() ;
	}
 
}
