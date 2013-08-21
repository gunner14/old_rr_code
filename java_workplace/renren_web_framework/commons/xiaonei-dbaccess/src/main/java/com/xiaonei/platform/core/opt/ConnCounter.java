/**
 * 
 */
package com.xiaonei.platform.core.opt;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author skllb
 * 
 */
public class ConnCounter
{
	private String name;
	private final AtomicInteger counter = new AtomicInteger(0);
	private int maxBlocked = MAX_BLOCKED;

	@Override
	public String toString()
	{
		return name + "|" + counter.intValue() + "|" + maxBlocked;
	}

	/**
	 * 默认值   
	 */
	private static int MAX_BLOCKED = 100;

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public AtomicInteger getCounter()
	{
		return counter;
	}

	public void setCounter(int counter)
	{
		this.counter.set(counter);
	}

	public int getMaxBlocked()
	{
		return maxBlocked;
	}

	public void setMaxBlocked(int maxBlocked)
	{
		this.maxBlocked = maxBlocked;
	}

	/**
	 * 进入模块
	 * 
	 * @return 如小于maxBlocked则允许继续进入，返回true,else return false
	 */
	public boolean enter()
	{
		if (maxBlocked > counter.get())
		{
			counter.incrementAndGet();
			return true;
		}

		return false;
	}

	/**
	 * 计数器--
	 */
	public void outer()
	{
		counter.decrementAndGet();
	}
}
