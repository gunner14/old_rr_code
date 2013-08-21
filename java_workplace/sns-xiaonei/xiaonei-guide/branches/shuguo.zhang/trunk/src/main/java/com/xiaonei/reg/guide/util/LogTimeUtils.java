package com.xiaonei.reg.guide.util;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;



public class LogTimeUtils {
	
	public final static Log logger = LogFactory.getLog(GuideData.class);

	public static long logEnd(long begin, String bizName)
	{
		if (begin > 0 && logger.isDebugEnabled())
		{
			long end = System.currentTimeMillis();
			logger.debug("Guide " + bizName + " cost time" + "|"
					+ (end - begin));
			return end;
		}
		return 0;
	}

	public static long logEnd(long begin, String bizName, int userid)
	{
		if (begin > 0 && logger.isDebugEnabled())
		{
			long end = System.currentTimeMillis();
			logger.debug("Guide " + bizName + " cost time" + "|"
					+ (end - begin) + "|" + userid);
			return end;
		}
		return 0;
	}

	public static long logBegin()
	{
		// 记录sql执行的开始时间
		if (logger.isDebugEnabled())
		{
			return System.currentTimeMillis();
		}
		return 0;
	}
}
