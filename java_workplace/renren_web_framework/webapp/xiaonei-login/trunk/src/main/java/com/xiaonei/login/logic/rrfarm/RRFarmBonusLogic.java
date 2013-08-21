package com.xiaonei.login.logic.rrfarm;

import java.util.List;

public interface RRFarmBonusLogic {

	/**
	 * notifyTheAppPlusBonus<br>
	 * 通知app平台加奖励
	 * 
	 * @param userId
	 * @param bonus
	 * 
	 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:20:04 PM
	 */
	public void notifyTheAppPlusBonus(int userId, RRFarmBonusModel bonus);

	/**
	 * addBonus<br>
	 * 
	 * @param userId
	 * @param bonus
	 * 
	 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:43:50 PM
	 */
	public void addBonus(int userId, RRFarmBonusModel bonus);

	/**
	 * sendNotificationToYourself<br>
	 * 发送站内通知给自己
	 * 
	 * @param userId
	 * @param bonus
	 * 
	 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:21:14 PM
	 */
	public void sendNotificationToYourself(int userId, RRFarmBonusModel bonus);

	/**
	 * getGivenBonuses<br>
	 * 获得已得到的奖励
	 * 
	 * @param hostId
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:20:56 PM
	 */
	public List<Integer> getGivenBonuses(int hostId);

	/**
	 * getRRFarmActivateDays<br>
	 * 参加活动的连续天数
	 * 
	 * @param userId
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Mar 15, 2010 - 9:38:05 AM
	 */
	public int getRRFarmActivateDays(int userId);
}
