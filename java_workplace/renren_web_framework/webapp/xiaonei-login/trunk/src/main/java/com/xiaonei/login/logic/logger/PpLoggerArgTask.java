package com.xiaonei.login.logic.logger;

/**
 * LoggerArgTask <br>
 * 
 * @author tai.wang@opi-corp.com Dec 29, 2009 - 6:03:03 PM
 */
public abstract class PpLoggerArgTask {

	public abstract Object work();

	@Override
	public String toString() {
		Object o = work();
		if (null != o) {
			return o.toString();
		} else {
			return "null";
		}

	}
}
