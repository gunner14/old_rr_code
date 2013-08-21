package com.xiaonei.passport.task.task;


import com.xiaonei.passport.task.followmission.IFollowMissionTask;
import com.xiaonei.xce.log.ActiveTrack;

/**
 * TODO ActiveTrack <br>
 * 
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 4:41:13 PM
 */
public class ActiveTrackTask implements IFollowMissionTask {

	private int userid;
	private String ip;
	private String requestUrl;

	public ActiveTrackTask(int userid, String ip, String requestUrl) {
		super();
		this.userid = userid;
		this.ip = ip;
		this.requestUrl = requestUrl;
	}

	@Override
	public void run() {
		try {
			ActiveTrack.access(userid, ip, requestUrl, null);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public String getName() {
		return "ActiveTrack";
	}

}
