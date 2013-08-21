package com.renren.xoa.server.catalina;

import java.io.IOException;

import org.apache.catalina.Session;
import org.apache.catalina.session.StandardManager;
import org.apache.catalina.session.StandardSession;
import org.apache.log4j.Logger;

/**
 * 自定义的Tomcat session manager，无论什么情况都返回一个固定的session实例，
 * 用以解决tomcat无法禁用session的问题
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-17 下午03:09:39
 */
public class IgnoreSessionManager extends StandardManager {
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	private Session defaultSession;
	
	public IgnoreSessionManager() {
		super();
		defaultSession = new StandardSession(this);
		if (logger.isTraceEnabled()) {
			logger.trace(this.getClass().getName() + " is constructed");
		}
	}
	
	@Override
	public Session createSession(String sessionId) {
		if (logger.isTraceEnabled()) {
			logger.trace("createSession:" + sessionId);
		}
		return defaultSession;
	}
	
	@Override
	public Session findSession(String sessionId) throws IOException {
		if (logger.isTraceEnabled()) {
			logger.trace("findSession:" + sessionId);
		}
		return defaultSession;
    }
	
	@Override
	public Session[] findSessions() {
		if (logger.isTraceEnabled()) {
			logger.trace("findSessions:");
		}
        return new Session[]{defaultSession};
    }
	
}
