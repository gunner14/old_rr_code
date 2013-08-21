/**
 * 
 */
package com.xiaonei.platform.component.portal;

import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * <ul>
 * <li>
 * portlet配置bean
 * 
 * @author skllb </li> <li>2009-3-19 下午11:29:28</li> <li></li>
 *         </ul>
 */
public class PortletBean
{
	// 是否使用
	private boolean used = true;

	// 模块名称
	private String name;

	// 执行的模块个数
	AtomicInteger exeingCounts = new AtomicInteger(0);

	// 最大并发执行的个数
	private int maxExecounts;

	/**
	 * 构造
	 * 
	 * @param name
	 * @param maxExecounts
	 *            最大的并发执行个数
	 * @param exe
	 */
	public PortletBean(String name, int maxExecounts, IPortletExecutor exe)
	{
		this.name = name;
		this.maxExecounts = maxExecounts;
		this.exe = exe;
	}

	// 需要初始化
	private IPortletExecutor exe = null;

	/**
	 * 是否已经达到最大并发数量
	 * 
	 * @return
	 */
	public boolean isOverMax()
	{
		return exeingCounts.intValue() > maxExecounts;
	}

	/**
	 * <ul>
	 * <li>是否使用</li>
	 * </ul>
	 * 
	 * @return
	 */
	public boolean isUsed()
	{
		return used;
	}

	/**
	 * <ul>
	 * <li>设定是否使用</li>
	 * </ul>
	 * 
	 * @param used
	 */
	public void setUsed(boolean used)
	{
		this.used = used;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public int getExeingCounts()
	{
		return exeingCounts.intValue();
	}

	public int getMaxExecounts()
	{
		return maxExecounts;
	}

	public void setMaxExecounts(int maxExecounts)
	{
		this.maxExecounts = maxExecounts;
	}

	public Map<String, Object> execute(PortalContext ctx)
	{
		// 自增一个正在执行的个数
		exeingCounts.incrementAndGet();

		Map<String, Object> re = null;
		try
		{
			// 开始执行
			re = exe.execute(ctx);
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			// 检查正在执行的个数
			exeingCounts.decrementAndGet();
		}

		return re;
	}

	public static void main(String[] args)
	{
		// final PortletBean pb = new PortletBean("", 10, null);
		//
		// for (int i = 0; i < 100000; i++)
		// {
		// new Thread(new Runnable()
		// {
		// @Override
		// public void run()
		// {
		// }
		// }).start();
		// // System.out.println(pb.exeingCounts);
		// }
	}

}
