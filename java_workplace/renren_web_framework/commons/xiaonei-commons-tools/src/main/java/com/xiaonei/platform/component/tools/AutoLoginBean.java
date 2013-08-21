/**
 * 
 */
package com.xiaonei.platform.component.tools;

import java.io.Serializable;
import java.util.Date;

/**
 * 用来登录
 * 
 * @author skllb
 * 
 */
public class AutoLoginBean implements Serializable
{
	private static final long serialVersionUID = 1L;

	public AutoLoginBean(Date time, int id)
	{
		this.time = time;
		this.id = id;
	}

	private Date time;
	private int id;

	public Date getTime()
	{
		return time;
	}

	public void setTime(Date time)
	{
		this.time = time;
	}

	public int getId()
	{
		return id;
	}

	public void setId(int id)
	{
		this.id = id;
	}
}
