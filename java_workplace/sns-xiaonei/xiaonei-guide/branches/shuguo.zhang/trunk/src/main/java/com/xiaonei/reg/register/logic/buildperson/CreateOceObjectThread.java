/**
 * xiaonei-reg
 * com.xiaonei.reg.register.logic.buildperson
 * CreateOceObjectThread.java
 * Sep 15, 2009 - 11:34:45 AM
 */
package com.xiaonei.reg.register.logic.buildperson;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.logic.TimeCost;

/**
 * CreateOceObjectThread
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 15, 2009 - 11:34:45 AM
 * 
 */
public abstract class CreateOceObjectThread extends Thread {

	private final RegLogger logger;
	private final Integer userId;
	private final String oceObjectName;

	public CreateOceObjectThread(final String oceObjectName,
			final Integer userId, final RegLogger logger) {
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
		final TimeCost cost = TimeCost.begin("create oce object "+userId);
		logger.info("create " + oceObjectName + ": " + userId + " start");
		long timeCostL = -1l;
		try {
			work();
			timeCostL = cost.endStep(oceObjectName);
		} catch (final Ice.LocalException e) {
			e.printStackTrace();
			logger.error("create " + oceObjectName + ": " + userId
					+ " exception", e);
		} catch (final Exception e) {
			e.printStackTrace();
			logger.error("create " + oceObjectName + ": " + userId
					+ " exception", e);
		} finally {
			logger.info("create " + oceObjectName + ": " + userId + " over, time cost: "+timeCostL);
			cost.endFinally();
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
