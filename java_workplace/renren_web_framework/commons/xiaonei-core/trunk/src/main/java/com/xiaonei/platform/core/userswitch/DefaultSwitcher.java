package com.xiaonei.platform.core.userswitch;

import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.dao.CoreConfigDAO;
import com.xiaonei.platform.core.schedule.ScheduleService;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午12:38:05
 */
public class DefaultSwitcher implements UserSwitch{
	
	private static final int RELOAD_DELAY = 60 * 5;
	
	private Log logger = LogFactory.getLog(this.getClass());
	
	private String id;

	/**
	 * 初始化的时候给controlCondition一个默认值，"1:2"是永远不可能成立的一个条件，也就是说默认关闭
	 */
	private ControlCondition controlCondition = new ControlCondition("1:2");
	
	private ScheduledExecutorService timer = ScheduleService.getService();
	
	public DefaultSwitcher() {
		
	}

	public void init() {
		startTimer();
	}
	
	private void startTimer() {
		try {
			timer.scheduleWithFixedDelay(new Runnable() {

				@Override
				public void run() {
					try {
						ControlCondition con = loadControlCondition(id);
						if (con != null) {
							controlCondition = con;
						}
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}, 0, RELOAD_DELAY, TimeUnit.SECONDS);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private ControlCondition loadControlCondition(String key) {
		if (logger.isDebugEnabled()) {
			logger.debug("reloading control condition:" + key);
		}
		try {
			String sCondition = CoreConfigDAO.getInstance().getCoreConfigValue(
					key);
			if (logger.isInfoEnabled()) {
				logger.info("new control condition loaded: " + key + "=" + sCondition);
			}
			return new ControlCondition(sCondition);
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return null;
	}
	
	@Override
	public String getId() {
		return id;
	}

	@Override
	public boolean isOn(int userId) {
		ControlCondition condition = this.controlCondition;

		if (condition == null) {
			return false;
		}

		int mod = condition.getMod();
		int[] remainders = condition.getRemainders();
		if (mod == 0 || remainders == null) {
			return false;
		}

		int remainder = userId % mod;
		for (int r : remainders) {
			if (r == remainder) {
				return true;
			}
		}
		return false;
	}

	public void setId(String id) {
		this.id = id;
	}
}
