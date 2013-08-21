package com.xiaonei.passport.logger;

import com.xiaonei.passport.constants.Globals;


public class PpLoggerMsg {

	private String groupName;

	public PpLoggerMsg(String groupName) {
		this.groupName = groupName;
	}

	String msg(Object s) {
		String msg;
		if (null == s) {
			msg = "";
		} else {
			// msg = StringUtils.replace(s.toString(), " ", "_");
			msg = s.toString();
		}
		if (!PpLoggerLocalMsg.isNotInitialValue()) {
			PpLoggerLocalMsg.set(String.format(" [%s] [%s] ", PpLoggerLocalMsg
					.get(), msg));
		}
		String localIp = Globals.localIp;
		return String.format("[%s] [%s] [%s] ", groupName, localIp, msg);
	}

}
