/**
 * 
 */
package com.xiaonei.platform.component.portal;


/**
 * <ul>
 * <li>
 * 健康检查线程
 * 
 * @author skllb </li> <li>2009-3-24 上午09:40:16</li> <li></li>
 *         </ul>
 */
public class HealthCheckMonitor
{
//	private static Log log = LogFactory.getLog("PORTLET_HELTHCHECK");
//
//	private IPortalStoreMgr mgr = null;
//	private int beginSleepping = 0;
//	private int stepSlepping = 0;

	/**
	 * 构造函数
	 * 
	 * @param mgr
	 *            配置库
	 * @param beginSleepping
	 *            启动时等待时间(ms),系统启动时，很多模块可能存在超时，所以启动时第一次等待时间应该长点
	 * @param stepSlepping
	 *            间隔等待时间， 我们将间隔等待时间内的超时次数认为是衡量一个模块是否健康的依据
	 */
	public HealthCheckMonitor(IPortalStoreMgr mgr, int beginSleepping,
			int stepSlepping)
	{
//		this.mgr = mgr;
//		this.beginSleepping = beginSleepping;
//		this.stepSlepping = stepSlepping;
	}

	public void start()
	{
		// new Thread(new Runnable()
		// {
		// @Override
		// public void run()
		// {
		// try
		// {
		// // 启动后等待
		// Thread.sleep(beginSleepping);
		//
		// while (true)
		// {
		// log
		// .info("HealthCheckMonitor Start............................................");
		//
		// for (PortletBean pb : mgr.getPortlets())
		// {
		// if (pb.isUsed())
		// {
		// if (pb.isAvaliable())
		// {
		// if (pb.isOverMax())
		// {// 堵塞数量超过最大值 ， 设置为非可用
		// pb.setAvaliable(false);
		// log.error(new StringBuilder().append(
		// "set ").append(pb.getName())
		// .append(" to unavaliable|")
		// .append(pb.getExeingCounts())
		// .append("|").append(
		// pb.getMaxExecounts())
		// .toString());
		// }
		// } else
		// {
		// // 是非可用的,进行ping检查
		//
		// IPortletExecutor ping = pb.getExecutor();
		//
		// long start = System.currentTimeMillis();
		// boolean canUse = false;
		// try
		// {
		// canUse = ping.ping();
		// } catch (Exception e)
		// {
		// e.printStackTrace();
		// }
		//
		// long cost = System.currentTimeMillis()
		// - start;
		//
		// if (canUse && !pb.isOverMax())
		// {// 可用， 且非超时
		// if (cost < pb.getTimeoutMinisec())
		// {
		// pb.setAvaliable(true);
		//
		// log.error(new StringBuilder()
		// .append("set ").append(
		// pb.getName())
		// .append("to availiable")
		// .toString());
		// }
		// }
		// }
		// }
		// }
		// log
		// .info("HealthCheckMonitor End............................................");
		// Thread.sleep(stepSlepping);
		// }
		// } catch (Exception e)
		// {
		// e.printStackTrace();
		// log.error("healthCheckMonitorError", e);
		// }
		// }
		// }).start();
	}
}
