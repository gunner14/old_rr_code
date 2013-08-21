package com.xiaonei.tools.profiler;

import com.xiaonei.xce.log.XceLogger;
import com.xiaonei.xce.log.XceLoggerAdapter;

public class ProfileLogger {
	private volatile static XceLogger logger = null;
	private ProfileLogger(){
		
	}
	
	public static XceLogger getXceLogger(){
		if(logger==null){
			synchronized(ProfileLogger.class){
				if(logger==null){
					logger = XceLoggerAdapter.getLogger("M:udp -h profilerlog -p 9000");
				}
			}
		}
		return logger;
	}
}
