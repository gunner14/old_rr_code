package com.renren.xoa.commons.env;

import javax.annotation.PostConstruct;
import javax.jms.IllegalStateException;

/**
 * 背景：由于现在（2011-1-19）各个工程中对外提供的jar包依赖不是很规范，
 * 有些甚至依赖的web上使用的xiaonei-commons-interceptors包等，造成
 * xoa的web工程可能会在不知情的情况下引用这些包，而这些包作用在xoa工程
 * 下，可能会出现很多未知的、不可控的问题。
 * 
 * 这个类存在的目的是，在初始化后({@link @PostConstruct})检查几个
 * xiaonei-commons-interceptors中存在的经典类，如果找到了，就认为
 * xiaonei-commons-interceptors包被错误地依赖到环境中来了，则向上
 * 抛出异常。
 * 
 * 此类需要被配置在applicationContext.xml中，以便能被Spring初始化。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-1-19 下午05:00:40
 */
public class EnvironmentChecker {

	public EnvironmentChecker() {
		
	}
	
	/**
	 * 待检查的xiaonei-commons-interceptors中的特征类
	 */
	private static final String[] checkedClasses = {
			"com.xiaonei.commons.interceptors.access.PassportValidationInterceptor",
			"com.xiaonei.commons.interceptors.access.track.AccessTrackInterceptor",
			"com.xiaonei.commons.interceptors.access.loginrequired.LoginRequiredInterceptor" };
	
	@PostConstruct
	public void check() throws IllegalStateException {
		try {
			for(String className : checkedClasses) {
				Class<?> klass = Class.forName(className);
				if (klass != null) {
					throw new IllegalStateException("Found both 'xiaonei-commons-interceptors-XXX.jar' and 'xoa-commons-interceptors-XXX.jar' in your WEB-INF/lib/. " + 
							"If it's a XOA web project, please REMOVE 'xiaonei-commons-interceptors-XXX.jar'. Otherwise, please REMOVE 'xoa-commons-interceptors-XXX.jar'");
				}
			}
		} catch (ClassNotFoundException e) {
			// do nothing
		}
	}
}
