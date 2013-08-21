/**
 * 
 */
package com.xiaonei.platform.component.portal;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * <ul>
 * <li>
 * 多线程执行
 * 
 * @author libin.liu </li> <li>2008-12-29 上午10:46:10</li> <li></li>
 *         </ul>
 */
public class ThreadsExecutor
{
	private static Log log = LogFactory.getLog("PORTLET_EXE");

	// 是否超时:::用来同步主线程与子线程的状态
	boolean isTimeout;

	/**
	 * cached 线程池::60秒未使用的线程自动关闭
	 */
	private static ExecutorService espool = Executors.newCachedThreadPool();

	// 并发执行的线程数
	private int threadCounts;

	// 最大等待次数
	private int maxWaitingTimes;

	// 正在执行的线程
	private int runningThread;

	// 判断线程是否结束时的等待时间
	private int mainThreadWatingStepMins;

	// 任务列表
	private List<PortletBean> tasks = null;

	// 执行环境
	private PortalContext ctx = null;

	/**
	 * 构造函数
	 * 
	 * @param threadCount
	 *            需要使用的总线程数
	 * @param mainThreadWatingStepMins
	 *            主线程单位等待时间：(ms)在开启threadCount个线程后，主线程将进入sleep，每次等待mainThreadWating毫秒
	 *            ，如传递0则表示主线程不等待，
	 * @param maxWaitingTime
	 *            主线程等待超时次数,等待时间~=mainThreadWatingStepMins * maxWaitingTimes
	 */
	public ThreadsExecutor(PortalContext ctx, int threadCounts,
			int mainThreadWatingStepMins, int maxWaitingTimes)
	{
		this.ctx = ctx;
		this.threadCounts = threadCounts;
		this.mainThreadWatingStepMins = mainThreadWatingStepMins;
		this.maxWaitingTimes = maxWaitingTimes;
	}

	/**
	 * <ul>
	 * <li>执行计算</li>
	 * <li>超时机制，现由线程自身来设定，可能造成一定的误差</li>
	 * </ul>
	 */
	public void execute(IPortalStoreMgr mgr)
	{
		long start = System.currentTimeMillis();

		if (this.threadCounts > 1)
		{// 多线程执行
			executeMutiThreads(mgr);
		} else
		{// 顺序执行
			executeSteps(mgr);
		}

		if (log.isDebugEnabled())
		{
			log.debug("TOTAL:\t" + (System.currentTimeMillis() - start));
		}
	}

	private void executeSteps(IPortalStoreMgr mgr)
	{
		// 单线程执行
		for (PortletBean pb : mgr.getPortlets())
		{// 使用所有可用的portlet来构建一个任务队列
			if (pb.isUsed())
			{// 需要使用

				if (!pb.isOverMax())
				{// 最大堵塞队列小于允许值
					long begin = System.currentTimeMillis();

					Map<String, Object> re = pb.execute(ctx);

					long cost = System.currentTimeMillis() - begin;

					if (re != null)
					{// 将所有结果加入到ctx 中
						ctx.addResults(re);
					}

					if (log.isDebugEnabled())
					{// 日志:::名称｜用时
						log.debug(new StringBuilder().append(pb.getName())
								.append("|").append(cost).toString());
					}
				} else
				{
					log.error(new StringBuilder().append(pb.getName()).append(
							" BLOCKED|").append(pb.getExeingCounts()).append(
							"|").append(pb.getMaxExecounts()));
				}
			}
		}
	}

	private void executeMutiThreads(IPortalStoreMgr mgr)
	{
		tasks = new LinkedList<PortletBean>();

		for (PortletBean pb : mgr.getPortlets())
		{// 使用所有可用的portlet来构建一个任务队列
			if (pb.isUsed())
			{// 需要使用
				if (!pb.isOverMax())
				{
					tasks.add(pb);
				} else
				{
					log.error(new StringBuilder().append(pb.getName()).append(
							" BLOCKED|").append(pb.getExeingCounts()).append(
							"|").append(pb.getMaxExecounts()));
				}
			}
		}
		// 将当前工作线程个数设定为允许设定的最大线程数
		runningThread = this.threadCounts;
        final String currentDomain = OpiConstants.getCurrentDomain();
		// 这里考虑各线程执行不可能有这么快了:::即建立threadCounts的时间内，不应该有线程已经把人物列表都已经跑完了，否则就不好完了，没有必要跑多线程了，呵呵。跑完了也不会产生影响的
		for (int i = 0; i < threadCounts /* && tasks.size()>0 */; i++)
		{
			espool.execute(new Runnable()
			{
				public void run()
				{
	                OpiConstants.setCurrentDomain(currentDomain);
					while (!isTimeout && tasks.size() > 0)
					{// 主线程没有超时的情况下，且任务列表中还有任务
						PortletBean pb = checkOutTask();

						if (pb != null)
						{
							long begin = System.currentTimeMillis();
							Map<String, Object> re = pb.execute(ctx);
							long cost = System.currentTimeMillis() - begin;

							if (re != null)
							{// 将所有结果加入到ctx 中
								ctx.addResults(re);
							}

							if (log.isDebugEnabled())
							{// 日志:::名称｜用时
								log.debug(new StringBuilder().append(
										pb.getName()).append("|").append(cost)
										.toString());
							}
						}
					}
					runningThread--;
				}
			});
		}

		if (mainThreadWatingStepMins > 0)
		{// 主要线程需要等待

			if (maxWaitingTimes > 0)
			{// 有等待超时限制
				int waittingTimes = 0;

				while (this.runningThread > 0 && !isTimeout)
				{
					waittingTimes++;
					isTimeout = maxWaitingTimes < waittingTimes;

					try
					{
						Thread.sleep(mainThreadWatingStepMins);
					} catch (InterruptedException e)
					{
						e.printStackTrace();
					}
				}

				if (runningThread > 0)
				{// 执行线程大于０,有部分服务未执行
					if (log.isWarnEnabled())
					{// 个别服务没有执行完成，可能有问题
						for (PortletBean pb : tasks)
						{// 警报日志：：：：名称，说明该模块未被执行
							log.warn(new StringBuilder().append(pb.getName())
									.append(" not execute").toString());
						}
					}
				}

			} else
			{// 没有等待超时
				while (this.runningThread > 0)
				{
					try
					{
						Thread.sleep(this.mainThreadWatingStepMins);
					} catch (InterruptedException e)
					{
						e.printStackTrace();
					}
				}
			}
		}
	}

	/**
	 * <ul>
	 * <li>从人物队列中取得一个任务</li>
	 * </ul>
	 * 
	 * @return
	 */
	private PortletBean checkOutTask()
	{
		PortletBean pb = null;

		synchronized (tasks)
		{// 从任务列表中取一个任务
			if (tasks.size() > 0)
			{
				pb = tasks.remove(0);
			}
		}
		return pb;
	}

	public static void main(String[] args)
	{
		// HttpServletRequest request = null;

		PortalContext ctx = new PortalContext(null);

//		ThreadsExecutor e = 
			new ThreadsExecutor(ctx, 5, 1, 10);
		// e.execute(ProfilePortletStoreMgrImpl.getInstance());

	}
}
