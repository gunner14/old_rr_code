package com.xiaonei.passport.logger;

/**
 * HighPerformancePpLogger <br>
 * 
 * @author tai.wang@opi-corp.com Dec 30, 2009 - 12:20:26 PM
 */
public interface HighPerformancePpLogger {

	/**
	 * info<br>
	 * 高性能的info
	 * 
	 * @param msg
	 * @param tasks
	 * 
	 * @author tai.wang@opi-corp.com Dec 29, 2009 - 6:23:55 PM
	 */
	public void info(final String msg, final Object... tasks);

	public void warn(final String msg, final Object... tasks);

	public void debug(final String msg, final Object... args);

	public void error(final String msg, final Object... args);

	public void trace(final String msg, final Object... args);
}
