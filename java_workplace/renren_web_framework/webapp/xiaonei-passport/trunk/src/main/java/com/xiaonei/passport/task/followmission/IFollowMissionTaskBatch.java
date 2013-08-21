package com.xiaonei.passport.task.followmission;

public interface IFollowMissionTaskBatch extends IFollowMissionTask {

	void batch(IFollowMissionTask task);
	
}
