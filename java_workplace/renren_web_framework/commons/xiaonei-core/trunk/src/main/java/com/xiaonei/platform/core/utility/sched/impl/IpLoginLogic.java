package com.xiaonei.platform.core.utility.sched.impl;

import com.xiaonei.platform.core.utility.sched.SchedManager;

public class IpLoginLogic {

	public static void saveIpLogin(int userId, String ip, int type) {
		try {
			IpLoginObject ilo = new IpLoginObject();
			ilo.setUserId(userId);
			ilo.setIp(ip);
			ilo.setType(type);
			IpLoginSched ils = new IpLoginSched(ilo);
			SchedManager.getInstance().addTask(ils);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
