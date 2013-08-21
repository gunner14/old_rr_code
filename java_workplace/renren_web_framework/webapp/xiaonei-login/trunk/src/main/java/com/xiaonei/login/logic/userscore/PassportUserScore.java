/**
 * 
 */
package com.xiaonei.login.logic.userscore;

/**
 * PassportUserScore <br>
 * 
 * @author tai.wang@opi-corp.com Mar 8, 2010 - 3:17:22 PM
 */
public interface PassportUserScore {

	public void incLoginCount(int userId, byte loginType);

	/**
	 * getLoginConsecutiveCount<br>
	 * 连续登陆天数
	 * 
	 * @param userId
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Mar 8, 2010 - 3:19:14 PM
	 */
	public int getLoginConsecutiveCount(int userId);

	public boolean isTodayLogined(int userId);
}
