package com.xiaonei.login.task.followmission;

public interface IFollowMissionTaskBatch extends IFollowMissionTask {

	void batch(IFollowMissionTask task);
	
}
