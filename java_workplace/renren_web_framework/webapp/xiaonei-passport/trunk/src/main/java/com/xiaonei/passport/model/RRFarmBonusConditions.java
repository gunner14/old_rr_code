/**
 * 
 */
package com.xiaonei.passport.model;

import java.util.List;

import com.xiaonei.passport.LoginLogicFactory;


/**
 * RRFarmBonusConditions <br>
 * 
 * @author tai.wang@opi-corp.com Mar 11, 2010 - 4:12:04 PM
 */
public class RRFarmBonusConditions {

	private int consecutiveDays;

	private boolean isSignedIn;

	private int userId;

	private int friendsNum;

	private List<Integer> bonusIdsList;

	private String domain;

	public RRFarmBonusConditions(int userId, boolean isSignedIn,
			int consecutiveDays, int friendsNum, String domain) {
		super();
		this.userId = userId;
		this.isSignedIn = isSignedIn;
		this.consecutiveDays = consecutiveDays;
		this.friendsNum = friendsNum;
		this.domain = domain;
	}

	public int getConsecutiveDays() {
		return consecutiveDays;
	}

	public int getUserId() {
		return userId;
	}

	public boolean isSignedIn() {
		return isSignedIn;
	}

	public int getFriendsNum() {
		return friendsNum;
	}

	public List<Integer> getBonusIdsList() {
		if (null == bonusIdsList) {
			bonusIdsList = LoginLogicFactory.getRRFarmLogicImpl().getGivenBonuses(
					userId);
		}
		return this.bonusIdsList;
	}

	public String getDomain() {
		return this.domain;
	}

	@Override
	public String toString() {
		return String
				.format(
						"RRFarmBonusConditions [consecutiveDays=%s, friendsNum=%s, isSignedIn=%s, userId=%s]",
						consecutiveDays, friendsNum, isSignedIn, userId);
	}

}
