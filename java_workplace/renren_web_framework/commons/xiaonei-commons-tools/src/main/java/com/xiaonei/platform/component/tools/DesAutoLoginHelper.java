/**
 * 
 */
package com.xiaonei.platform.component.tools;

import java.util.Date;

import org.apache.commons.lang.math.NumberUtils;

import com.xiaonei.platform.core.utility.DES_Encrypt;

/**
 * @author skllb
 * 
 */
public class DesAutoLoginHelper
{
	private static byte[] key = "_(+E&xaiFDesS1'E/".getBytes();

	// 三天有效
	private static long validateTime = 1000 * 60 * 60 * 24 * 3;

	private DesAutoLoginHelper()
	{
	}

	public static String encrypt(int id)
	{
		long now = System.currentTimeMillis();
		String temp = now + "_" + id;

		try
		{
			return DES_Encrypt.byte2hex(DES_Encrypt.encrypt(temp.getBytes(),
					key));
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return null;
	}

	public static Date decrypt(String str)
	{
		if (str == null || "".equals(str))
		{
			return null;
		}
		try
		{
			String value = new String(DES_Encrypt.decrypt(DES_Encrypt
					.hex2byte(str.getBytes()), key));
			String[] vs = value.split("_");
			if (vs.length == 2)
			{
				long time = NumberUtils.toLong(vs[0]);
				int id = NumberUtils.toInt(vs[1]);
				if (time > 0 && id > 0)
				{// 必须都合法
					long t = System.currentTimeMillis() - time;
					Date sendTime = null;
					if (t > 0 && t < validateTime)
					{// 时间有效
						sendTime = new Date(t);
					}
					else
					{
						
					}
				}
			}

		} catch (Exception e)
		{
			e.printStackTrace();
		}

		return null;
	}

	public static void main(String[] args)
	{
		System.out.println(DesAutoLoginHelper.encrypt(1111111111));
		System.out.println(DesAutoLoginHelper.decrypt(DesAutoLoginHelper
				.encrypt(1111111111)));

	}

}
