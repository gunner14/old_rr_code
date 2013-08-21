/**
 * 
 */
package com.xiaonei.reg.thread.timelimit.exception;


/**
 * Timeout Exception
 * 
 * @author wangtai
 * 
 */
public class TimeLimitTimeoutException extends TimeLimitException {

	private static final long serialVersionUID = 3802730823664785152L;

	public TimeLimitTimeoutException(Throwable e) {
		super(e);
	}

	public TimeLimitTimeoutException(String e) {
		super(e);
	}

}