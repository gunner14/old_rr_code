/*
 * Created on 2003-12-16
 */
package com.xiaonei.platform.core.utility.sched;

import com.xiaonei.platform.core.utility.MathUtils;



/**
 * ���������Ϊһ��Schedulable�ӿ�ʵ���Proxy����һ���������
 * ͬʱ������񱻵�ȴ��������ʱ���������Ϣ
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public class SchedTask {
	
	protected int schedTimes = -1;
	
	protected int execTimes = 0;
	
	protected Schedulable task;
	
	protected SchedTaskQueue taskQueue;
	
	protected long lastSched;
	
	protected String taskId;
	
	protected boolean firstExec = true;
	 
	public SchedTask(SchedTaskQueue queue, Schedulable sched) {
		this.taskQueue = queue;
		this.task = sched;
		this.schedTimes = sched.getSchedTimes();
		this.taskId = MathUtils.MD5(sched.getDescription() + System.currentTimeMillis());
	}
	
	public void execute() {
		try {
			this.task.run();
		} catch ( Exception e ) {
			e.printStackTrace();
		}
		
		if (schedTimes == 0) {
			// finished
			return;
		} else if (this.schedTimes > 0) {
			this.schedTimes--;
			if ( this.schedTimes == 0 ) {
				return;
			}
		}
		// schedule again
		this.taskQueue.releaseTask(this);
	}
	
	public boolean timeToExec() {
		if (this.schedTimes != 0) {
			long curr = System.currentTimeMillis();
			if ( (curr - this.lastSched) >= this.task.getInterval() ) {
				this.lastSched = curr;
				if (!this.firstExec) {
					firstExec = true;
					return false;
				}
				this.execTimes++;
				return true;
			}
		}		
		return false;
	}
	
	public String getTaskId() {
		return this.taskId;
	}
	
	public String toString() {
		return "Task : " + task.getDescription();
	}

	/**
	 * @param b
	 */
	public void setFirstExec(boolean b) {
		firstExec = b;
	}

}
