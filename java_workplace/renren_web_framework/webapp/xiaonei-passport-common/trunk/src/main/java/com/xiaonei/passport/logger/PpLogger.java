/**
 * 
 */
package com.xiaonei.passport.logger;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;


/**
 * PassportLogger
 * 
 * @author tai.wang@opi-corp.com Dec 22, 2009 - 6:54:25 PM
 */
public class PpLogger implements HighPerformancePpLogger {

	private static class Delegate {

		/** delegate */
		public HighPerformancePpLoggerImpl hpLogger;

		/** delegate */
		public Logger logger;

		/** delegate */
		public PpLoggerMsg loggerMsg = new PpLoggerMsg("");
	}

	private Delegate dg = new Delegate();

	private boolean enable = true;

	PpLogger(Logger logger, String groupName) {
		dg.hpLogger = new HighPerformancePpLoggerImpl();
		dg.hpLogger.setLogger(logger);
		dg.logger = logger;
		if (StringUtils.isNotEmpty(groupName)) {
			dg.loggerMsg = new PpLoggerMsg(groupName);
			dg.hpLogger.setLoggerMsg(dg.loggerMsg);
		}
		if (StringUtils.equals(dg.logger.getName(),
				LoginLoggerDef.LOGIN_LOGGER_NAME)) {
			dg.logger.setLevel(PassportLoggerConfiguration.level);
		}
	}

	public void debug(Object s, Throwable throwable) {
		if (enable) {
			dg.logger.debug(msg(s), throwable);
		}
	}

	@Override
	public void debug(String msg, Object... args) {
		if (enable) {
			dg.hpLogger.debug(msg, args);
		}
	}

	public void error(Object s, Throwable throwable) {
		if (enable) {
			dg.logger.error(msg(s), throwable);
		}
	}

	@Override
	public void error(String msg, Object... args) {
		if (enable) {
			dg.hpLogger.error(msg, args);
		}

	}

	public void info(Object s, Throwable throwable) {
		if (enable) {
			dg.logger.info(msg(s), throwable);
		}
	}

	@Override
	public void info(String msg, Object... tasks) {
		if (enable) {
			dg.hpLogger.info(msg, tasks);
		}
	}

	public boolean isDebugEnabled() {
		return dg.logger.isDebugEnabled();
	}

	public boolean isEnable() {
		return enable;
	}

	public boolean isInfoEnabled() {
		return dg.logger.isInfoEnabled();
	}

	public boolean isTraceEnabled() {
		return dg.logger.isTraceEnabled();
	}

	public PpLogger setEnable(boolean enable) {
		this.enable = enable;
		return this;
	}

	public void trace(Object s, Throwable throwable) {
		if (enable) {
			dg.logger.trace(msg(s), throwable);
		}
	}

	@Override
	public void trace(String msg, Object... args) {
		if (enable) {
			dg.hpLogger.trace(msg, args);
		}
	}

	public void warn(Object s, Throwable throwable) {
		if (enable) {
			dg.logger.warn(msg(s), throwable);
		}
	}

	@Override
	public void warn(String msg, Object... tasks) {
		if (enable) {
			dg.hpLogger.warn(msg, tasks);
		}
	}

	private Object msg(Object s) {
		return dg.loggerMsg.msg(s);
	}

}
