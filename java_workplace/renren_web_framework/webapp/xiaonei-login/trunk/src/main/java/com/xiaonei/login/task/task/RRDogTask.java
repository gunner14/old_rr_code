package com.xiaonei.login.task.task;

import java.io.IOException;

import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.rrfarm.AppBonusLogicImpl;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * RRDogTask <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 7:46:44 PM
 */
public class RRDogTask implements IFollowMissionTask {

	private String domain;
	private int userid;

	public RRDogTask(String domain, int userid) {
		super();
		this.domain = domain;
		this.userid = userid;
	}

	@Override
	public String getName() {
		return "RRDogTask";
	}

	@Override
	public void run() {
		if (LoginLogicFactory.getPassportUserScore().isTodayLogined(userid)) {
			return;
		}
		String tDomain = domain;
		try {
			User user = SnsAdapterFactory.getUserAdapter().get(userid);
			if (null != user) {
				tDomain = user.getDomain();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (!"kaixin.com".equals(tDomain)) {
			return;
		}
		addbones(userid, 500);
	}

	private void addbones(int userid, int bones) {
		String sUrl = String
				.format(
						"http://dog.kaixin.com/rewardbones.do?key=886971c7961950114e97b8f2963f1e10&method=1&encourage_bones=%s&uid=%s",
						bones, userid);
		try {
			AppBonusLogicImpl.wgetUrl(sUrl);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
}
