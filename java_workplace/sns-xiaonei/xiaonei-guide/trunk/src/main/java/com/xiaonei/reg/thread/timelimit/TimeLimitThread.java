package com.xiaonei.reg.thread.timelimit;

public abstract class TimeLimitThread extends
		TimeLimitConfiguration.InnerTLThread {

	public TimeLimitThread(TimeLimitConfiguration tl) {
		tl.super();
	}

	@Override
	public abstract Object work();

}
