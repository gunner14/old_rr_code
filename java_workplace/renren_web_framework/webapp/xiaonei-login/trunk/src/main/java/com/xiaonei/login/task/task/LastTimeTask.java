package com.xiaonei.login.task.task;

import java.util.Date;

import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.xce.usertime.UserTimeInfo;

/**
 * TODO LastTimeTask <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 4:39:38 PM
 */
public class LastTimeTask implements IFollowMissionTask {

	private int userid;

	public LastTimeTask(int userid) {
		super();
		this.userid = userid;
	}

	@Override
	public void run() {
		if (!LoginLogicFactory.getPassportUserScore().isTodayLogined(userid)) {
			WUserAdapter.getInstance().getLoginCountAdapter()
					.incUserLoginCount(userid);
			UserTimeInfo time = new UserTimeInfo();
			time.setId(userid);
			time.setLastLoginTime(new Date());
			WUserDescAdapter.getInstance().getTimeAdapter().setUserTime(userid,
					time);
		}
	}

	@Override
	public String getName() {
		return "LastTime";
	}

}
