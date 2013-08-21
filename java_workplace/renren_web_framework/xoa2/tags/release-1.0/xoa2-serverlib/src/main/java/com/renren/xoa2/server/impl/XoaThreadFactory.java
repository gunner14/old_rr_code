package com.renren.xoa2.server.impl;

import java.lang.Thread.UncaughtExceptionHandler;
import java.util.concurrent.ThreadFactory;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * 对于出现异常的服务线程进行合适的处理
 * 
 * @author yuan.liu1@renren-inc.com
 */
public class XoaThreadFactory implements ThreadFactory {
	/**
	 * 错误处理，暂时仅仅进行日志打印
	 * 
	 * TODO: 其他需要的策略   
	 */
	class XoaUncaughtExceptionHandler implements UncaughtExceptionHandler {
		@Override
		public void uncaughtException(Thread t, Throwable e) {
			Logger logger = LoggerFactory.getLogger(XoaUncaughtExceptionHandler.class);
			logger.error("Service thread exception/error in : " + t.getName(), e);
		}
	}
	
	public XoaThreadFactory() {
		Thread.setDefaultUncaughtExceptionHandler(null);
	}

	@Override
	public Thread newThread(Runnable r) {
		Thread thread = new Thread(r);
		thread.setUncaughtExceptionHandler(new XoaUncaughtExceptionHandler());
		return thread;
	}
}
