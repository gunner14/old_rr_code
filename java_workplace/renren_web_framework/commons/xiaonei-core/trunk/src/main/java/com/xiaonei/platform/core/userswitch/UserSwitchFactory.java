package com.xiaonei.platform.core.userswitch;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午12:37:22
 */
public class UserSwitchFactory {

	private static SwitchPool pool = new SwitchPool();
	
	public static UserSwitch get(String switchId) {
		return pool.get(switchId);
	}
}
