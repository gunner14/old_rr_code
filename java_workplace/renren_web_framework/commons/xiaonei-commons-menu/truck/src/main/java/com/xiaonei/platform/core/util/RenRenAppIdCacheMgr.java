/**
 *   人人自产App的ID cache类
 *   @creator     liu.yu@renren-inc.com
 *   @create-time 2012-4-24   下午7:06:27   
 *   @revision    $Id
 */
package com.xiaonei.platform.core.util;

import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.component.application.cache.AppBasicCacheMgr;
import com.xiaonei.platform.component.application.home.AppBaseHome;
import com.xiaonei.platform.component.application.model.AppBasic;

/**
 * 站内应用ID本地缓存
 * 
 * @author liu.yu@renren-inc.com 2012-9-13-上午11:54:31
 */
public class RenRenAppIdCacheMgr {

	private final static Log logger = LogFactory.getLog(RenRenAppIdCacheMgr.class);

	private final ReentrantReadWriteLock lock = new ReentrantReadWriteLock();

	private final ReadLock rlock = lock.readLock();

	private final WriteLock wlock = lock.writeLock();

	private Set<Integer> renrenAppIds = new HashSet<Integer>();

	private static final int RELOAD_DELAY_IN_MINITUE = 30;

	private ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

	private static RenRenAppIdCacheMgr instance = new RenRenAppIdCacheMgr();

	private RenRenAppIdCacheMgr() {
		initRenRenAppIds(renrenAppIds);
		executor.scheduleWithFixedDelay(new Runnable() {

			@Override
			public void run() {
				initRenRenAppIds(renrenAppIds);
			}

		}, RELOAD_DELAY_IN_MINITUE, RELOAD_DELAY_IN_MINITUE, TimeUnit.MINUTES);
	}

	private void initRenRenAppIds(Set<Integer> target) {
		try {
			wlock.lock();

			List<Integer> ids = AppBaseHome.getInstance().getRenRenApps();
			for (Integer temp : ids) {
				target.add(temp);
			}
			if (logger.isInfoEnabled()) {
				logger.info("reload renren appIds ,all renren app is :" + target.size());
			}
		} finally {
			wlock.unlock();
		}
	}

	public static RenRenAppIdCacheMgr getInstance() {
		return instance;
	}

	public Set<Integer> getRenRenAppIds() {
		try {
			rlock.lock();
			return new HashSet<Integer>(renrenAppIds);
		} finally {
			rlock.unlock();
		}
	}

	public int[] getRrAppIds() {
		try {
			rlock.lock();

			if (this.renrenAppIds == null) {
				return new int[0];
			}
			int[] appIds = new int[renrenAppIds.size()];
			int offset = 0;
			for (int appId : renrenAppIds) {
				appIds[offset++] = appId;
			}
			return appIds;
		} finally {
			rlock.unlock();
		}
	}

	/**
	 * 需要过滤的人人应用（除去connect类型的）
	 * 
	 * @param userMenu
	 * @return
	 */
	public int[] getRenRenAppsExceptConnect() {
		Set<Integer> appIds = RenRenAppIdCacheMgr.getInstance().getAppIds();
		int[] renrenApps = new int[appIds.size()];
		int offset = 0;

		for (Integer appId : appIds) {
			if (appId != null) {
				AppBasic basic = AppBasicCacheMgr.getInstance().get(appId);
				if (basic != null && basic.isConnect()) {
					continue;
				}
				renrenApps[offset++] = appId;
			}
		}

		return ArrayUtils.subarray(renrenApps, 0, offset);
	}

	public Set<Integer> getAppIds() {
		try {
			rlock.lock();
			if (this.renrenAppIds == null) {
				return Collections.emptySet();
			}
			return new HashSet<Integer>(renrenAppIds);
		} finally {
			rlock.unlock();
		}
	}

	public boolean isRenRenApp(Integer appId) {
		return this.getRenRenAppIds().contains(appId);
	}

	public static void main(String[] args) throws InterruptedException {
		for (int i = 0; i < 1000; i++) {
			Thread t = new Thread(new Runnable() {

				@Override
				public void run() {
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

					System.out.println(RenRenAppIdCacheMgr.getInstance().getRrAppIds()[0]);
				}
			});
			Thread.sleep(100);
			t.start();
		}
	}
}
