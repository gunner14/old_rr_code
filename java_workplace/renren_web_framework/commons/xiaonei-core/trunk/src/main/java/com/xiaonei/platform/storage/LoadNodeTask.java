package com.xiaonei.platform.storage;

import java.util.TimerTask;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class LoadNodeTask extends TimerTask {
	
	private static Log logger = LogFactory.getLog(TimerTask.class);
	
	private Domain domain;
	public LoadNodeTask(Domain domain) {this.domain = domain;}
	public void run() {
		try {
			if (logger.isInfoEnabled()) {
				logger.info("Time Time Time load node task start..........................");
			}
			domain.loadConfig();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
