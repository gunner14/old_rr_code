package com.xiaonei.platform.core.userswitch;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午02:45:18
 */
public class UserSwitchIDs {

	public static final String SWITCH_ID_UGCTAB = "user_switch.ugctab";
	
	public static void main(String[] args) {
		
		UserSwitch switcher = UserSwitchFactory.get(UserSwitchIDs.SWITCH_ID_UGCTAB);
		int userId = 1234;
		if (switcher.isOn(userId)) {
			//your logic here
		}
	}
	
}
