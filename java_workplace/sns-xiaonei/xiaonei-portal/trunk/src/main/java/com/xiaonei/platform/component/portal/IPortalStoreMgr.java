/**
 * 
 */
package com.xiaonei.platform.component.portal;

import java.util.Collection;

/**
 * <ul>
 * <li>
 * portlet仓库
 * 
 * @author skllb </li> <li>2009-3-20 上午12:05:31</li> <li></li>
 *         </ul>
 */
public interface IPortalStoreMgr
{

	/**
	 * <ul>
	 * <li>获取所有的portlets</li>
	 * </ul>
	 * 
	 * @return
	 */
	public Collection<PortletBean> getPortlets();

	/**
	 * 获得执行线程数
	 * 
	 * @return
	 */
	public int getThreadCounts();

	/**
	 * 设定执行线程数
	 * 
	 * @param threadCounts
	 */
	public void setThreadCounts(int threadCounts);

	/**
	 * 获得最大等待次数
	 * 
	 * @return
	 */
	public int getMaxWaitingTimes();

	/**
	 * 设定最大等待次数
	 * 
	 * @param maxWaitingTimes
	 */
	public void setMaxWaitingTimes(int maxWaitingTimes);

	/**
	 * 主线程单次等待时间
	 * 
	 * @return
	 */
	public int getMainThreadWatingStepMins();

	/**
	 * 设定主线程单次等待时间
	 * 
	 * @param mainThreadWatingStepMins
	 */
	public void setMainThreadWatingStepMins(int mainThreadWatingStepMins);
}
