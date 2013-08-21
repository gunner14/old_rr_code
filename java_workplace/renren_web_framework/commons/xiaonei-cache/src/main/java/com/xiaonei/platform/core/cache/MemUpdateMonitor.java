package com.xiaonei.platform.core.cache;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;

public class MemUpdateMonitor {

	private static final int UPDATE_INTERVAL = 60 * 2;// two min

	private static Log logger = LogFactory.getLog(MemUpdateMonitor.class);

	private String lastUpdateTime = null;

	private volatile boolean stop = false;
	private static String status = "show table status like 'memconfig2'";

	private static ScheduledExecutorService timer = Executors
			.newSingleThreadScheduledExecutor(new MyThreadFactory());

	public void stopMonitor() {
		stop = true;
	}

	public void start() {
		if (logger.isInfoEnabled()) {
			logger.info("xiaonei-cache config monitor is starting...");
		}
		// 每2分钟检查一次Pool的初始化状况。每4分钟检查一下配置更新。
		timer.scheduleWithFixedDelay(new Runnable() {
			@Override
			public void run() {
				if (!stop) {
					try {
						MemCacheFactory.getInstance().checkPoolInit();
						if (isUpdated()) {
							if (logger.isInfoEnabled()) {
								logger
										.info("xiaonei-cache reloading config...");
							}
							MemCacheFactory.getInstance()
									.update(lastUpdateTime);
						} else {
							if (logger.isInfoEnabled()) {
								logger.info("xiaonei-cache config not updated");
							}
						}
					} catch (Exception ex) {
						ex.printStackTrace();
					}
				} else {
					if (logger.isInfoEnabled()) {
						logger.info("xiaonei-cache config monitor is stopped");
					}
				}
			}
		}, UPDATE_INTERVAL, UPDATE_INTERVAL, TimeUnit.SECONDS);
	}

	private boolean isUpdated() throws SQLException {
		OpUniq op = new OpUniq(status, DataAccessMgr.BIZ_STAT) {
			public Object parse(ResultSet rs) throws SQLException {
				return rs.getString("Update_time");
			}
		};

		String stamp = (String) DataAccessMgr.getInstance().queryUnique(op);

		if (lastUpdateTime == null) {
			lastUpdateTime = stamp;
			return false;
		} else {
			if (stamp.equals(lastUpdateTime))
				return false;
			if (logger.isInfoEnabled()) {
				logger.info("last=" + lastUpdateTime + ":now=" + stamp);
			}
			lastUpdateTime = stamp;
			return true;
		}
	}

}