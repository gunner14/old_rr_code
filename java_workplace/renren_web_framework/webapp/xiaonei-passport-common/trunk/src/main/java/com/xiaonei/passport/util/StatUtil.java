package com.xiaonei.passport.util;

import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;

public class StatUtil {

	public static String format = "%s cost : %d";

	long start = System.currentTimeMillis();

	long time = System.currentTimeMillis();

	PpLogger log = null;

	public StatUtil() {
		this.log = LoginLoggerDef.tstat;
	}

	public StatUtil(PpLogger log2) {
		if (log2 != null) {
			this.log = log2;
		} else {
			this.log = LoginLoggerDef.tstat;
		}
	}

	public long stepTimeCost(String desc) {
		return stepTimeCost(desc, null);
	}

	public long getWholeTime(String desc) {
		return getWholeTime(desc, null);
	}

	public long getWholeTime(String desc, Object append) {
		long time = System.currentTimeMillis() - start;
		log(time, desc, append);
		return time;
	}

	public long stepTimeCost(String desc, Object append) {
		log(System.currentTimeMillis() - time, desc, append);
		time = System.currentTimeMillis();
		return time;
	}

	public void log(long time, String desc, Object append) {
		if (log.isDebugEnabled()) {
			if (append != null) {
				log.debug(String.format(format, desc, time) + ":" + append);
			} else {
				log.debug(String.format(format, desc, time));
			}
		}
	}
}
