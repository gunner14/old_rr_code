package com.xiaonei.passport.task.followmission;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public interface FollowMissionQueueDef {

	BlockingQueue<IFollowMissionTask> queue = new LinkedBlockingQueue<IFollowMissionTask>();

}
