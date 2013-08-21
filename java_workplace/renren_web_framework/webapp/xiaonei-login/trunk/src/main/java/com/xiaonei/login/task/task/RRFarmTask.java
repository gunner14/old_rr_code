package com.xiaonei.login.task.task;

import java.util.List;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.logic.rrfarm.RRFarmBonusConditions;
import com.xiaonei.login.logic.rrfarm.RRFarmBonusModel;
import com.xiaonei.login.logic.userscore.PassportUserScore;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.platform.component.friends.home.FriendsHome;

/**
 * TODO RRFarmTask <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 7:46:51 PM
 */
public class RRFarmTask implements IFollowMissionTask {

	private int userid;
	private String domain;
	private PpLogger log = LoginLoggerDef.flyerTask;

	public RRFarmTask(int userid, String domain) {
		super();
		this.userid = userid;
		this.domain = domain;
	}

	@Override
	public String getName() {
		return "RRFarmTask";
	}

	@Override
	public void run() {
		int consecutiveDays = 0;
		try {
			consecutiveDays = LoginLogicFactory.getRRFarmLogicImpl()
					.getRRFarmActivateDays(userid);
		} catch (Exception e) {
			e.printStackTrace();
		}
		int friendsNum = 0;
		try {
			friendsNum = FriendsHome.getInstance().getFCount(userid);
		} catch (Exception e) {
			e.printStackTrace();
		}
		PassportUserScore passportUserScore = getPasssportUserScore();
		// 收集奖励条件
		RRFarmBonusConditions conditions = new RRFarmBonusConditions(userid,
				passportUserScore.isTodayLogined(userid), consecutiveDays,
				friendsNum, domain);
		List<RRFarmBonusModel> listBonus = LoginLogicFactory.getRRFarmLogicImpl()
				.getBonus(conditions);

		// 循环发奖励
		RRFarmBonusModel notifybonus = null;
		for (RRFarmBonusModel bonus : listBonus) {
			notifybonus = bonus;
			LoginLogicFactory.getRRFarmLogicImpl().addBonus(userid, bonus);
			log.info("add bonus " + userid + ". conditions is "
					+ conditions.toString() + " bonus is " + bonus.toString());
		}
		if (notifybonus != null) {// 只发一条通知
			LoginLogicFactory.getRRFarmLogicImpl().sendNotificationToYourself(
					userid, notifybonus);
		}
		log.info("add bonus " + userid + ". conditions is "
				+ conditions.toString());
	}

	private PassportUserScore getPasssportUserScore() {
		PassportUserScore passportUserScore;
		try {
			passportUserScore = LoginLogicFactory.getPassportUserScore();
		} catch (Exception e1) {
			e1.printStackTrace();
			try {
				passportUserScore = LoginLogicFactory.getPassportUserScore();
			} catch (Exception e) {
				e.printStackTrace();
				return null;
			}
		}
		return passportUserScore;
	}

}
