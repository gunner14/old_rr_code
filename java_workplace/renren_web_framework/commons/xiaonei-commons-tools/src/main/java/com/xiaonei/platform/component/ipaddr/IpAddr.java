/**
 * 
 */
package com.xiaonei.platform.component.ipaddr;

/**
 * @author skllb
 * 
 */
public class IpAddr implements Comparable<IpAddr>
{
	public long getStart()
	{
		return start;
	}

	public void setStart(long start)
	{
		this.start = start;
	}

	public long getEnd()
	{
		return end;
	}

	public void setEnd(long end)
	{
		this.end = end;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public String getCode()
	{
		return code;
	}

	public void setCode(String code)
	{
		this.code = code;
	}

	/**
	 * 开始区段
	 */
	private long start;

	/**
	 * 截至区段
	 */
	private long end;

	/**
	 * 地区（中文）
	 */
	private String name;
	
	/**
	 * 编码
	 */
	private String code;

	@Override
	public int compareTo(IpAddr o)
	{
		return this.start > o.start ? 1 : (this.start < o.start ? -1 : 0);
	}
}
