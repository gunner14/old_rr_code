package com.xiaonei.platform.framework.log;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 只是一个标识接口，实现此接口的类可以方便的使用log
 * @author lifs
 *
 */
public interface XnLog {

	public static Log logger = LogFactory.getLog(XnLog.class);
	
}
