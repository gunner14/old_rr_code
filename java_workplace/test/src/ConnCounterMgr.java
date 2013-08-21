/**
 * 
 */
package com.xiaonei.platform.core.opt;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.lang.math.NumberUtils;

import com.xiaonei.platform.core.opt.ConnCounter;

/**
 * 计数器管理类,简单包装一下， 可供dbConn和iceConn等其他服务使用
 * 
 * @author skllb
 * 
 */
public class ConnCounterMgr
{
	// 计数器
	private static Map<String, ConnCounter> counters = new HashMap<String, ConnCounter>();

	private ConnCounterMgr()
	{

	}

	static
	{
		try
		{
			final Properties p = new Properties();
			p.load(ConnCounterMgr.class.getClassLoader().getResourceAsStream(
			"ConnCounter.properties"));
			for (final Object keyt : p.keySet())
			{
				final String key = (String) keyt;
				final String value = p.getProperty(key);

				if ("IS_USED_COUNTER".equals(key))
				{
					if ("1".equals(value) || "true".equalsIgnoreCase(value))
					{
						ConnCounterMgr.used = true;
					} else
					{
						ConnCounterMgr.used = false;
					}
					System.out.println("init ConnCounterService:\tisUsed:" + ConnCounterMgr.used);
				} else
				{
					final int valueInt = NumberUtils.toInt(value);
					if (valueInt > 0)
					{
						final ConnCounter counter = new ConnCounter();
						counter.setName(key);
						counter.setMaxBlocked(valueInt);
						counters.put(key, counter);
						System.out.println("init ConnCounterService:\t" + key + "\t" + valueInt);
					}
				}
			}
		} catch (final Exception e)
		{// 可以无该文件,使用默认值即可
		}


		try {
			final ScheduledExecutorService svc = Executors
					.newSingleThreadScheduledExecutor();
			svc.schedule(new Runnable() {
				@Override
				public void run() {
					try {
						for (final Entry<String, ConnCounter> entry : counters
								.entrySet()) {
							entry.getValue().setCounter(0);
						}
					} catch (final Throwable e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}, 1, TimeUnit.HOURS);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


	}

	// 是否使用计数服务,默认为false
	private static boolean used;

	public static boolean isUsed()
	{
		return used;
	}

	/**
	 * 进入计数器
	 * 
	 * @param name
	 * @return
	 */
	public static boolean enter(String name)
	{
		if (!used)
			return true;

		ConnCounter conter = counters.get(name);
		if (conter != null)
			return conter.enter();
		else
		{// 如果没有则新加一个:::这里性能问题可以忽略，因为只有第一次初始化e：））））
			synchronized (counters)
			{
				conter = counters.get(name);
				if (conter != null)
					return conter.enter();

				conter = new ConnCounter();
				conter.setName(name);
				conter.enter();
				counters.put(name, conter);
				return true;
			}
		}
	}

	/**
	 * 退出计数器,自减
	 * 
	 * @param name
	 */
	public static void outer(String name)
	{
		if (!used)
			return;

		final ConnCounter conter = counters.get(name);
		if (conter != null)
		{
			conter.outer();
		} else
		{
			new IllegalArgumentException(name).printStackTrace(System.err);
		}
	}

	/**
	 * 
	 * @return
	 */
	public static Map<String, ConnCounter> getCounters()
	{
		return counters;
	}

	/**
	 * 获取某个计数器
	 * 
	 * @param name
	 * @return
	 */
	public static ConnCounter get(String name)
	{
		return counters.get(name);
	}
}
