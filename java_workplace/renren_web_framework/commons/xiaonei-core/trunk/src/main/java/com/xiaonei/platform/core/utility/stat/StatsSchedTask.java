/*
 * Created on 2004-1-15
 */
package com.xiaonei.platform.core.utility.stat;

import java.util.Date;

import com.xiaonei.platform.core.utility.StringUtils;
import com.xiaonei.platform.core.utility.sched.SchedTask;
import com.xiaonei.platform.core.utility.sched.SchedTaskQueue;
import com.xiaonei.platform.core.utility.sched.Schedulable;


/**
 * ��Ϊ��ÿ��02:00:00���һ��
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class StatsSchedTask extends SchedTask {
	
	public int SCHED_TIME = 020000;

	public StatsSchedTask(SchedTaskQueue queue, Schedulable sched) {
		super(queue, sched);
	}
	
	public void setSchedTime(int time) {
		this.SCHED_TIME = time;
	}

	/* (non-Javadoc)
	 * @see com.dudu.sched.SchedTask#timeToExec()
	 */
	public boolean timeToExec() {
		if (this.schedTimes != 0) {
			long curr = System.currentTimeMillis();
			Date date =  new Date(curr);
			Date last = new Date(this.lastSched);
			
			String today = StringUtils.formatDate(date, "yyyyMMdd");
			String yesterday = StringUtils.formatDate(last, "yyyyMMdd");			
			if (today.equals(yesterday)) {
				return false;
			}
			
			int timeVal = Integer.parseInt(StringUtils.formatDate(date, "HHmmss"));
			if (timeVal >= SCHED_TIME && (timeVal - SCHED_TIME) < 1500) {
				this.lastSched = curr;
				this.execTimes++;
				return true;
			}
		}
		return false;
	}

}
