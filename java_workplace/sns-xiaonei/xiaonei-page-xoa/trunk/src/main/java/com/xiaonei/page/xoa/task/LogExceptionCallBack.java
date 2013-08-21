package com.xiaonei.page.xoa.task;

import com.danga.MemCached.Logger;


public class LogExceptionCallBack implements CallBack{
	private final static Logger logger=Logger.getLogger("PAGEXOA");

	@Override
	public void exceptionCall(Exception e, String attachMsg) {
		logger.error(attachMsg, e);
	}

	@Override
	public void successCall() {
	}

	@Override
	public void failedCall() {	
	}
	
}
