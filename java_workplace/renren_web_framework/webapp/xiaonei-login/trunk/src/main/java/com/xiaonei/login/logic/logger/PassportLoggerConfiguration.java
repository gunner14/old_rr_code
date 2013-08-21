package com.xiaonei.login.logic.logger;

import java.io.IOException;

import org.apache.log4j.AsyncAppender;
import org.apache.log4j.DailyRollingFileAppender;
import org.apache.log4j.Layout;
import org.apache.log4j.Level;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.apache.log4j.PatternLayout;
import org.apache.log4j.net.SocketAppender;

import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;

public class PassportLoggerConfiguration {

	static {
		// configuration
		if (isTestEnv()) {
			level = Level.TRACE;
			port = 58778;
		} else {
			level = Level.INFO;
			port = 58778;
		}
		remoteHost = "10.3.18.189";
		reconDelay = 5000;
		datePattern = "'.'yyyyMMdd";
		filename = "/data/resin/logs/passport/passport.log";
		pattern = "%d{yyyy-MM-dd HH:mm:ss} [%p] [%c{1}] %m %n";

		// Initialization
		LOGGER = LogManager.getLogger(LoginLoggerDef.LOGIN_LOGGER_NAME);
		LOGGER_PassportValidationInterceptor = LogManager
				.getLogger("com.xiaonei.commons.interceptors.access.PassportValidationInterceptor");
		initPassportLogger();
	}

	/**
	 * initPassportLogger<br>
	 * 初始化passpert logger 配置
	 * 
	 * 
	 * @author tai.wang@opi-corp.com Dec 25, 2009 - 3:06:23 PM
	 */
	public static void initPassportLogger() {
		System.out.println("Initializing passport logger");
		SocketAppender socketAppender = new SocketAppender(remoteHost, port);
		System.out.println("soket appender:");
		System.out.println("remote host: " + remoteHost);
		System.out.println("port: " + port);
		socketAppender.setReconnectionDelay(reconDelay);
		System.out.println("reconection delay: " + reconDelay);
		socketAppender.setLocationInfo(true);

		DailyRollingFileAppender dailyAppender = null;
		if (isTestEnv()) {
			System.out.println("daily rolling file appender:");
			try {
				Layout layout = new PatternLayout(pattern);
				System.out.println("patten: " + pattern);
				dailyAppender = new DailyRollingFileAppender(layout, filename,
						datePattern);
				System.out.println("filename: " + filename);
				System.out.println("date pattern: " + pattern);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		AsyncAppender asyncAppender = new AsyncAppender();
		asyncAppender.addAppender(socketAppender);

		/* appender */
		LOGGER.removeAllAppenders();
		LOGGER.setAdditivity(false);
		LOGGER.setLevel(level);
		System.out.println("logger level: " + level);
		LOGGER.addAppender(asyncAppender);
		if (null != dailyAppender) {
			if (isTestEnv()) {
				LOGGER.addAppender(dailyAppender);
			}
		}
		System.out.println("Initialization passport logger end ");

		LOGGER_PassportValidationInterceptor.removeAllAppenders();
		LOGGER_PassportValidationInterceptor.setAdditivity(false);
		LOGGER_PassportValidationInterceptor.addAppender(asyncAppender);
		LOGGER_PassportValidationInterceptor.setLevel(Level.ERROR);
		if (null != dailyAppender) {
			if (isTestEnv()) {
				LOGGER_PassportValidationInterceptor.addAppender(dailyAppender);
			}
		}

	}

	private static Boolean isTestEnv() {
		return "10.3.18.206".equals(Globals.localIp)// 
				|| "10.3.18.189".equals(Globals.localIp)//
				|| System.getProperty("DEBUG_ENV") != null //
		;
	}

	public static Level level;

	public static String remoteHost;

	public static int port;

	public static int reconDelay;

	public static String datePattern;

	public static String filename;

	public static String pattern;

	static Logger LOGGER;

	private static Logger LOGGER_PassportValidationInterceptor;
}
