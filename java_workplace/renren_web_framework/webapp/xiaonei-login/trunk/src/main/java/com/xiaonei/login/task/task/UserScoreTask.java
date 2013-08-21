package com.xiaonei.login.task.task;

import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.wservice.userscore.login.LoginType;

/**
 * TODO UserScoreTask <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 4:23:52 PM
 */
public class UserScoreTask implements IFollowMissionTask {

	private int userid;
	private byte loginType = LoginType.TYPE_WEB;

	public UserScoreTask(int userid) {
		super();
		this.userid = userid;
	}

	public UserScoreTask(int userid, byte loginType) {
		this(userid);
		this.loginType = loginType;
	}

	@Override
	public void run() {
//		LoginLoggerDef.login.info("user score" + userid);
		// 获取真正的userId,来精确计算用户登陆次数
		int realUserId = SnsAdapterFactory.getUserAdapter().get(userid).getRealUserId();
		if (realUserId > 0) {
			this.userid = realUserId;
		}
		LoginLogicFactory.getPassportUserScore().incLoginCount(userid,
				loginType);
		
	}

	@Override
	public String getName() {
		return "UserScore";
	}

}
