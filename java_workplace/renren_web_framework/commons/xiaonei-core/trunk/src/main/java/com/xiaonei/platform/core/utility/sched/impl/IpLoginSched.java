package com.xiaonei.platform.core.utility.sched.impl;

import com.xiaonei.platform.core.utility.sched.Schedulable;

public class IpLoginSched implements Schedulable {

	private Object source;

	public IpLoginSched(Object source) {
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
		IpLoginObject ilo = (IpLoginObject) source;
		try {
			IpLoginDAO.getInstance().save(ilo);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
