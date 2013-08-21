package com.xiaonei.passport.logger;

import org.apache.log4j.Logger;

import com.xiaonei.passport.threadpool.RunnableTaskExecutor;


public class HighPerformancePpLoggerImpl implements HighPerformancePpLogger {

	private enum Level {
		debug, error, info, trace, warn
	}

	private Logger logger;

	private PpLoggerMsg loggerMsg = new PpLoggerMsg("");

	public Logger getLogger() {
		return logger;
	}

	public PpLoggerMsg getLoggerMsg() {
		return loggerMsg;
	}

	public void setLogger(Logger logger) {
		this.logger = logger;

	}

	public void setLoggerMsg(PpLoggerMsg loggerMsg) {
		this.loggerMsg = loggerMsg;

	}

	@Override
	public void debug(final String msg, final Object... args) {
		this.logHp(Level.debug, msg, args);
	}

	@Override
	public void error(final String msg, final Object... args) {
		this.logHp(Level.error, msg, args);
	}

	@Override
	public void info(final String msg, final Object... args) {
		this.logHp(Level.info, msg, args);
	}

	@Override
	public void trace(final String msg, final Object... args) {
		this.logHp(Level.trace, msg, args);
	}

	@Override
	public void warn(final String msg, final Object... args) {
		this.logHp(Level.warn, msg, args);
	}

	/**
	 * logHp <br>
	 * high performance
	 * 
	 * @param level
	 * @param msg
	 * @param args
	 * 
	 * @author tai.wang@opi-corp.com Dec 29, 2009 - 7:45:22 PM
	 */
	private void logHp(final Level level, final String msg,
			final Object... args) {
		Runnable logTask = new Runnable() {

			@Override
			public void run() {
				if (null == args) {
					logSwitch(level, msg);
					return;
				}
				if (null != args && 0 == args.length) {
					logSwitch(level, msg);
					return;
				}
				if (null == msg) {
					logSwitch(level, "null");
					return;
				}
				Object[] args1 = new Object[args.length == 0 ? 0 : args.length];
				for (int i = 0; i < args.length; i++) {
					if (args[i] instanceof PpLoggerArgTask) {
						args1[i] = ((PpLoggerArgTask) args[i]).work();
					} else {
						args1[i] = args[i];
					}
				}
				try {
					logSwitch(level, String.format(msg, args1));
				} catch (Exception e) {
					logSwitch(level, msg);
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTaskForLogger(logTask);
	}

	private void logSwitch(final Level level, final String msg) {
		String msg2 = (String) msg(msg);
		if (PpLoggerLocalMsg.isNotInitialValue()) {
			if (Level.info.equals(level)) {
				this.logger.info(msg2);
			}
			if (Level.warn.equals(level)) {
				this.logger.warn(msg2);
			}
			if (Level.debug.equals(level)) {
				this.logger.debug(msg2);
			}
			if (Level.error.equals(level)) {
				this.logger.error(msg2);
			}
			if (Level.trace.equals(level)) {
				this.logger.trace(msg2);
			}
		}
	}

	private Object msg(Object s) {
		return loggerMsg.msg(s);
	}
}
