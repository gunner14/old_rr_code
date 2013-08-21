package com.xiaonei.login.task.task;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.platform.core.utility.Encoder;

public class RecordMd5Pwd implements IFollowMissionTask {

	protected PpLogger remoteLogger = LoginLoggerDef.logMima;

	private String email;

	private String pwd;

	public RecordMd5Pwd(String email, String pwd) {
		this.email = email;
		this.pwd = pwd;
	}

	@Override
	public String getName() {
		return "RecodeMd5Pwd";
	}

	@Override
	public void run() {
		if (email != null) {
			if (remoteLogger.isEnable()) {
				String md51 = Encoder.encode(pwd + "XeD-bByH-jJKA-AVzC");
				String md52 = Encoder.encode(pwd
						+ "1aEa0l1aEGVl0YOeCQJ2E8htqPd");
				remoteLogger.info(email + " " + md51 + " " + md52);
			}
		}
	}
}
