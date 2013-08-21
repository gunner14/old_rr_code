/*
 * Created on 2004-1-15
 */
package com.xiaonei.platform.core.utility.stat;

import com.xiaonei.platform.core.utility.sched.SchedManager;



/**
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public abstract class StatsManager  {
	
	private StatsManager instance;
	
	public StatsManager getInstance() {
		return instance;
	}
	
	/**
	 * ��һ��ͳ�Ƶ�����������б�
	 * 
	 * @param sched
	 */
	public void addStatsTask(StatsSched sched) {
		SchedManager schMan = SchedManager.getInstance();
		schMan.addDailyStatsTask(sched);
	}
	
	
	
	/**
	 * �õ���ͳ�����ͼ���ַ
	 * 
	 * @param itemName
	 * @param year
	 * @param month
	 * @return
	 */
	public abstract String getMonthReportPic(String itemName, int year, int month);

	/* (non-Javadoc)
	 * @see org.apache.avalon.framework.configuration.Configurable#configure(org.apache.avalon.framework.configuration.Configuration)
	 */
	

}
