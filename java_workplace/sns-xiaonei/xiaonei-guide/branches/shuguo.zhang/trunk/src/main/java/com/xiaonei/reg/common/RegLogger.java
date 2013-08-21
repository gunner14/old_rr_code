package com.xiaonei.reg.common;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.log4j.Logger;

/**
 * log4j 几个常用操作进行封装，统一log格式，加快加载log4j速度
 * <p/>
 * Logger
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-5-31 - 上午10:53:29
 */
public class RegLogger {

	private boolean DEBUG_ENV = System.getProperty("DEBUG_ENV") != null;

	/* 新的log4j 加在这里，命名规则，log4j_name + "Logger" */
	@SuppressWarnings("unused")
	private static Logger regLogLogger = Logger.getLogger("regLog");

	/* 静态变量区 */
	private static final String REG_DEFAULT_LOG = "regLog";
	private static SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd"
			+ "." + "HH:mm:ss");
	private static String SPLIT = " ";
	private static String DEBUG = "[DEBUG]";
	private static String INFO = "[INFO]";
	private static String WARN = "[WARN]";
	private static String ERROR = "[ERROR]";
	private String EMAIL = "[tai.wang@opi-corp.com]";

	public void setEmail(String email) {
		this.EMAIL = email;
	}

	/* 属性 */
	private Logger logger = null;
	private String logtype = null;

	private RegLogger(String loggerName, String Type) {
		logtype = "[" + Type + "]";
		try {
			logger = (Logger) this.getClass().getDeclaredField(
					loggerName + "Logger").get(logger);
		} catch (RuntimeException e) {
			logger = Logger.getLogger(loggerName);
		} catch (IllegalAccessException e) {
			logger = Logger.getLogger(loggerName);
		} catch (NoSuchFieldException e) {
			logger = Logger.getLogger(loggerName);
		}
	}

	public static RegLogger getLogger(String loggerName, String loggerType) {
		return new RegLogger(loggerName, loggerType);
	}

	public static RegLogger getLoggerWithType(String loggerType) {
		return new RegLogger(REG_DEFAULT_LOG, loggerType);
	}

	public static RegLogger getLoggerWithType(String loggerType, String email) {
		RegLogger log = new RegLogger(REG_DEFAULT_LOG, loggerType);
		log.setEmail(email);
		return log;
	}

	public static RegLogger getLoggerWithType(Class<? extends Object> clazz) {
		return new RegLogger(REG_DEFAULT_LOG, clazz.getSimpleName());
	}

	/**
	 * @deprecated
	 * @param loggerName
	 * @return
	 */
	public static RegLogger getLoggerWithName(String loggerName) {
		return new RegLogger(loggerName, "DEFAULT");
	}

	/**
	 * @deprecated
	 * @return
	 */
	public static RegLogger getLogger() {
		return new RegLogger(REG_DEFAULT_LOG, "DEFAULT");
	}

	public void setLogType(String logType) {
		this.logtype = logType;
	}

	public void debug(String debug) {
		logger.debug(DEBUG + SPLIT + EMAIL + SPLIT + df.format(new Date())
				+ SPLIT + logtype + SPLIT + debug);
		if (DEBUG_ENV) {
			logger.info(DEBUG + SPLIT + EMAIL + SPLIT + df.format(new Date())
					+ SPLIT + logtype + SPLIT + debug);
		}
		// System.out.println(DEBUG + SPLIT + EMAIL + SPLIT + df.format(new
		// Date())
		// + SPLIT + logtype + SPLIT + info);
	}

	public void info(String info) {
		logger.info(INFO + SPLIT + EMAIL + SPLIT + df.format(new Date())
				+ SPLIT + logtype + SPLIT + info);
	}

	public void warn(String warn) {
		logger.warn(WARN + SPLIT + EMAIL + SPLIT + df.format(new Date())
				+ SPLIT + logtype + SPLIT + warn);
	}

	public void error(String error) {
		logger.error(ERROR + SPLIT + EMAIL + SPLIT + df.format(new Date())
				+ SPLIT + logtype + SPLIT + error);
	}

	public void error(String error, Throwable e) {
		logger.error(ERROR + SPLIT + EMAIL + SPLIT + df.format(new Date())
				+ SPLIT + logtype + SPLIT + error, e);
	}

	public void error(Throwable e) {
		logger.error(ERROR + SPLIT + EMAIL + SPLIT + df.format(new Date())
				+ SPLIT + logtype + SPLIT, e);
	}
}
