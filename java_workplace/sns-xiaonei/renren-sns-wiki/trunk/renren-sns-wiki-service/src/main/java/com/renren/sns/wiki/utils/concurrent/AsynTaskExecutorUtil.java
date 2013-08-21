package com.renren.sns.wiki.utils.concurrent;

/**
 * 异步任务执行器工具类
 * 
 * @author yi.li@renren-inc.com since 2011-12-5
 * 
 */
public class AsynTaskExecutorUtil {

	private static volatile AsynTaskExecutor exeInstance;
	private static Object lock = new Object();

	/**
	 * 获取异步执行器（单例）
	 * 
	 * @return
	 */
	public static AsynTaskExecutor getAsynExecutor() {
		if (exeInstance == null) {
			synchronized (lock) {
				if (exeInstance == null) {
					exeInstance = new AsynTaskExecutor();
				}
			}
		}

		return exeInstance;
	}
}
