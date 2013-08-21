/*
 * Created on 2003-12-17
 */
package com.xiaonei.platform.core.utility.sched;

import java.util.Date;

/**
 * �������ʵ�ּ򵥾���
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class MySched implements Schedulable {

	/* (non-Javadoc)
	 * @see com.dudu.sched.Schedulable#getSchedTimes()
	 */
	public int getSchedTimes() {
		// TODO Auto-generated method stub
		return 1;
	}

	/* (non-Javadoc)
	 * @see com.dudu.sched.Schedulable#getInterval()
	 */
	public long getInterval() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see com.dudu.sched.Schedulable#getDescription()
	 */
	public String getDescription() {
		// TODO Auto-generated method stub
		return "MyTest";
	}

	/* (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
		// TODO Auto-generated method stub
		System.out.println("Test schedule." + (new Date()).toLocaleString());
	}

}
