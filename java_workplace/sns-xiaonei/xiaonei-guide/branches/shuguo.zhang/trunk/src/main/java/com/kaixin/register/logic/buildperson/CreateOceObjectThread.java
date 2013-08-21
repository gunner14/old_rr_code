/**
 * kaixin-reg
 * com.dodoyo.register.logic.buildperson
 * CreateOceObjectThread.java
 * Sep 15, 2009 - 11:34:45 AM
 */
package com.kaixin.register.logic.buildperson;

import org.apache.log4j.Logger;


/**
 * CreateOceObjectThread
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 15, 2009 - 11:34:45 AM
 * 
 */
public abstract class CreateOceObjectThread extends Thread {

	private final Logger logger;
	private final Integer userId;
	private final String oceObjectName;

	public CreateOceObjectThread(final String oceObjectName,
			final Integer userId, final Logger logger) {
		this.logger = logger;
		this.userId = userId;
		this.oceObjectName = oceObjectName;
	}

	public String getOceObjectName() {
		return oceObjectName;
	}

	public Integer getUserId() {
		return userId;
	}

	@Override
	public void run() {
		logger.info("create " + oceObjectName + ": " + userId + " start");
		try {
			work();
		} catch (final Ice.LocalException e) {
			e.printStackTrace();
			logger.error("create " + oceObjectName + ": " + userId
					+ " exception", e);
		} catch (final Exception e) {
			e.printStackTrace();
			logger.error("create " + oceObjectName + ": " + userId
					+ " exception", e);
		} finally {
			logger.info("create " + oceObjectName + ": " + userId + " over");
		}
	}

	/**
	 * work
	 * 
	 * create oce 小对象
	 * 
	 * @author wangtai(tai.wang@opi-corp.com) Sep 15, 2009 - 11:35:52 AM
	 */
	protected abstract void work();
}
