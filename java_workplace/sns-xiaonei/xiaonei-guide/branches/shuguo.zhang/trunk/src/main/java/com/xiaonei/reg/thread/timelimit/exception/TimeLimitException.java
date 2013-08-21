/**
 * 
 */
package com.xiaonei.reg.thread.timelimit.exception;

import java.io.PrintStream;

/**
 * Timeout Exception
 * 
 * @author wangtai
 * 
 */
public class TimeLimitException extends Exception {

	private static final long serialVersionUID = 3802730823664785152L;

	public TimeLimitException(Throwable e) {
		super(e);
	}

	public TimeLimitException(String e) {
		super(e);
	}

	@Override
	public String getMessage() {
		return "Time limit thread timeout.";
	}

	@Override
	public void printStackTrace() {
//		super.printStackTrace();
		// RegLogger.getLoggerWithType(this.getClass()).error(this);
	}

	@Override
	public void printStackTrace(PrintStream s) {
		synchronized (s) {
			if (getCause() != null)
				super.printStackTrace(s);
			else {
				s.println(this);
				StackTraceElement[] trace = getStackTrace();
				for (int i = 0; i < 4; i++)
					s.println("\tat " + trace[i]);
			}
		}
	}

}
