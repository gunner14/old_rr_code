/*
 * Created on 2004-1-15
 */
package com.xiaonei.platform.core.utility.stat;

import com.xiaonei.platform.core.utility.sched.Schedulable;



/**
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public abstract class StatsSched implements Schedulable {

	public static final long DAY_LEN = 24 * 3600 * 1000;
	
	/* (non-Javadoc)
	 * @see com.dudu.sched.Schedulable#getSchedTimes()
	 */
	public int getSchedTimes() {
		return -1;
	}

	/* (non-Javadoc)
	 * @see com.dudu.sched.Schedulable#getInterval()
	 */
	public long getInterval() {
		return DAY_LEN;
	}

	/* (non-Javadoc)
	 * @see com.dudu.sched.Schedulable#getDescription()
	 */
	public abstract String getDescription();

	/**
	 * run()������ÿ�챻����һ�Σ��ڱ������У�
	 * ��¼������ݵ���ݿ⣻
	 * ���£��ػ棩����ͳ��ͼ��
	 * 
	 * @see java.lang.Runnable#run()
	 */
	public abstract void run();
	
	/**
	 * ÿ��ͳ����ݵ�ͼ�ŵ�һ��Ŀ¼�У�����·ݱ���ͼ���ļ����磺
	 * ${context-root}/admin/stats/pic/newuser/male/2004-01-21.png
	 * 
	 * @return month pic store path
	 */
	public abstract String getStorePath();

}
