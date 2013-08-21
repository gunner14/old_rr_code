package com.xiaonei.passport.task.task;

import com.xiaonei.passport.LoginLogicFactory;
import com.xiaonei.passport.task.followmission.IFollowMissionTask;
import com.xiaonei.wservice.userscore.login.LoginType;

/**
 * TODO UserScoreTask <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 4:23:52 PM
 */
public class UserScoreTask implements IFollowMissionTask {

	private int userid;
	private byte loginType = LoginType.TYPE_WEB;



	public UserScoreTask(int userid, byte loginType) {
		this.userid = userid;
		this.loginType = loginType;
	}

	@Override
	public void run() {
//		LoginLoggerDef.login.info("user score" + userid);
		LoginLogicFactory.getPassportUserScore().incLoginCount(userid,
				loginType);
	}

	@Override
	public String getName() {
		return "UserScore";
	}

}
