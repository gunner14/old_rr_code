package com.xiaonei.platform.core.userswitch;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午12:37:29
 */
public interface UserSwitch {

	/**
	 * @return 当前Switch实例的ID
	 */
	public String getId();
	
	/**
	 * 当前开关是否对当前用户开放
	 * 
	 * @param userId
	 * @return
	 */
	public boolean isOn(int userId);
	
}
