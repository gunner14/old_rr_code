package com.xiaonei.reg.guide.root.logics;

import java.sql.SQLException;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.dao.StepStatusDAO;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * StepStatusLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午05:22:33
 */
public class StepStatusLogic {


	private static final String MEM_CACHE_GUIDE_STATUS_ID = "mem_cache_guide_status_";
	private static StepStatusLogic instance = new StepStatusLogic();

	public static StepStatusLogic getInstance() {
		return instance;
	}

	private StepStatusLogic() {
		super();
	}
	/**
	 * 更新状态值
	 * 
	 * @param userId
	 * @param step
	 * @return 更新是否成功
	 */
	public boolean updateStepStatus(final int userId, final int step) {
		boolean isOK = true;
		MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_user_space);
		String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
		GuideLogger.printLog("host:" + userId + " - " + key);
		try {
			mem.set(key, Integer.valueOf(step | getStatusById(userId)),	(int) MemCacheKeys.minute * 30);
		} catch (Exception e) {
			GuideLogger.printLog("save err host:" + userId + " step=["	+ step + "]" + e.toString(), GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		GuideLogger.printLog("save host:" + userId + " step=[" + step + "]");
		Runnable task_save = new Runnable() {
			public void run() {
				try {
					StepStatusDAO.getInstance().save(userId, step);
				} catch (SQLException e) {
					GuideLogger.printLog("save err host:" + userId + " step=["	+ step + "]" + e.toString(), GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(task_save);
		return isOK;
	}

	/**
	 * 取状态值
	 * 
	 * @param userId
	 * @return
	 * @throws SQLException
	 */
	public int getStatusById(int userId)  {
		int status = 0;
		// 先从mem cache中取
		MemCacheManager mem = MemCacheFactory.getInstance()
				.getManagerByPoolName(MemCacheKeys.pool_user_space);
		String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
		Object obj = mem.get(key);
		try {
			status = ((Integer) obj).intValue();
		} catch (NullPointerException e) {
			GuideLogger.printLog("exception accured - "+status);
		}
		// 如果取不到，把从库中取，并把结果放到mem cache中
		if (obj == null) {
			GuideLogger.printLog("obj is null");
			try {
				status = StepStatusDAO.getInstance().getStatus(userId);
				GuideLogger.printLog("from db :"+status);
			} catch (Exception e) {
				GuideLogger.printLog("db exception accurd:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			mem.set(key, Integer.valueOf(status), (int) MemCacheKeys.minute * 30);
		}
		return status;
	}



}

