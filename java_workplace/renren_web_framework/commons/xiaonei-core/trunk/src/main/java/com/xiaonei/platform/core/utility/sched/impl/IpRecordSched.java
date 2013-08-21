package com.xiaonei.platform.core.utility.sched.impl;

import com.xiaonei.platform.core.utility.sched.Schedulable;

public class IpRecordSched implements Schedulable {

	private Object source;

	public IpRecordSched(Object source) {
		this.source = source;
	}

	public String getDescription() {
		// TODO Auto-generated method stub
		return null;
	}

	public long getInterval() {
		// TODO Auto-generated method stub
		return 0;
	}

	public int getSchedTimes() {
		// TODO Auto-generated method stub
		return 1;
	}

	public void run() {
		// TODO Auto-generated method stub
		IpRecordObject iro = (IpRecordObject) source;
		try {
			IpRecordDAO.getInstance().save(iro);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
