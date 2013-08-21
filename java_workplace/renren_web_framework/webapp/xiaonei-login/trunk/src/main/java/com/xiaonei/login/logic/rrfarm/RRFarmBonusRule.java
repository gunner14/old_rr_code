/**
 * 
 */
package com.xiaonei.login.logic.rrfarm;

import java.util.List;

/**
 * RRFarmBonusRule <br>
 * 
 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:42:01 PM
 */
public interface RRFarmBonusRule {

	/**
	 * getBonus<br>
	 * bonus rule
	 * 
	 * @param conditions
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:21:49 PM
	 */
	public List<RRFarmBonusModel> getBonus(RRFarmBonusConditions conditions);

	public String getNotification(String userName, RRFarmBonusModel model);

}
