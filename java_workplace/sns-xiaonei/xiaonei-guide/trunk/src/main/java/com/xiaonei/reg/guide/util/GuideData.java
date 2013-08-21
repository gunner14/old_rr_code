package com.xiaonei.reg.guide.util;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class GuideData {

	private final static Log logger = LogFactory.getLog(GuideData.class);
	protected void logEnd(long begin,String bizName){
		if(begin >0 && logger.isDebugEnabled()){
			long end = System.currentTimeMillis();
			logger.debug("Guide "+bizName+" cost time" + "|"+(end-begin));
		}
	}

	protected long logBegin(){
		//记录sql执行的开始时间
		if(logger.isDebugEnabled()){
			return System.currentTimeMillis();
		}
		return 0;
	}
}
