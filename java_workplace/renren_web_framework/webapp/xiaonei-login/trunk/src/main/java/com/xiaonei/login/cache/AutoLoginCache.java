/**
 * 
 */
package com.xiaonei.login.cache;

import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

/**
 *	用户自动登录，记录用户id是否是当天第一次登陆cache 
 *
 * @author wen.he1
 * @createTime 2012-7-25 下午09:31:54 
 * 
 */
public class AutoLoginCache extends AbstractCommonCache{
	private static TripodCacheClient autoLoginCacheClient;
	private static AutoLoginCache autoLoginCache;
	
	static {
		autoLoginCacheClient = TripodCacheClientFactory.getClient(EntryType.BIZ_LOGIN_STEPS_AD); 

		autoLoginCache = new AutoLoginCache();
	}	
	public static AbstractCommonCache getInstance(){ 
		return autoLoginCache; 
	}
	
	@Override
	public TripodCacheClient getClient() { 
		return autoLoginCacheClient;
	}
	
}
