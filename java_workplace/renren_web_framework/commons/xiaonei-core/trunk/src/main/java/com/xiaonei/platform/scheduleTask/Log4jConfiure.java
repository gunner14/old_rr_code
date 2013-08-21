package com.xiaonei.platform.scheduleTask;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class Log4jConfiure {
	public static void logger(Class classname, String dubugContent) {
		PropertyConfigurator
				.configure("/root/upload_monitor/log4j.properties");
		Logger logger = Logger.getLogger(classname);
		logger.info(dubugContent);
	}

	/*public static void main(String args[]) {
		Log4jConfiure.logger(Log4jConfiure.class, "你想做什么呀，兄弟");
	}*/
}