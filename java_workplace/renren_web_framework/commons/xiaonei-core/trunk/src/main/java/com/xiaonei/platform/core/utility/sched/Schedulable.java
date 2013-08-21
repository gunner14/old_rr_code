/*
 * Created on 2003-12-16
 */
package com.xiaonei.platform.core.utility.sched;

/**
 * 
 * class MySched implements Schedulable ...
 * 
 * MySched msch = new MySched(...); <br>
 * com.dudu.SchedManager schMan = com.dudu.SchedManager.getInstance(); <br>
 * schMan.addTask(msch); <br>
 * 
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public interface Schedulable extends Runnable {
	
	/**
	 * ������count
	 *count
	 * 
	 * @return schedule times of this task
	 */
	public int getSchedTimes();

	/**
	 * �ȸ������ʱ����
	 * 
	 * @return time interval
	 */
	public long getInterval();
	
	/**
	 * ��������
	 * 
	 * @return description of this task
	 */
	public String getDescription();
	
	/**
	 * 
	 */
	public void run();
	
}
