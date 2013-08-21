package com.xiaonei.login.logic.logger;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.xiaonei.login.constants.LoginLoggerDef;

public class PassportLoggerFactory {

	/**
	 * getLogger
	 * 
	 * @param groupName
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Dec 22, 2009 - 7:28:12 PM
	 */
	public static PpLogger getLogger(String groupName, boolean enable) {
		String log = LoginLoggerDef.LOGIN_LOGGER_NAME;
		String key = log + "-" + groupName;
		if (LOGGER_MAP.get(key) != null) {
			return LOGGER_MAP.get(key);
		}
		PpLogger logger = new PpLogger(PassportLoggerConfiguration.LOGGER,
				groupName).setEnable(enable);
		LOGGER_MAP.put(key, logger);
		return logger;
	}

	private static final Map<String, PpLogger> LOGGER_MAP = new ConcurrentHashMap<String, PpLogger>();

	public static Map<String, PpLogger> getLoggerMap() {
		return LOGGER_MAP;
	}

}
