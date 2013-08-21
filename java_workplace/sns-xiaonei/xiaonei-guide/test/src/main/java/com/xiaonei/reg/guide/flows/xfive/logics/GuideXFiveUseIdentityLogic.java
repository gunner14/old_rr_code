package com.xiaonei.reg.guide.flows.xfive.logics;

import java.sql.SQLException;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.dao.GuideUserIdentityDAO;
import com.xiaonei.reg.guide.util.GuideLogger;

public class GuideXFiveUseIdentityLogic {
    private static GuideXFiveUseIdentityLogic instance = new GuideXFiveUseIdentityLogic();
    private static final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
	private static final String MEM_CACHE_GUIDE_IDENTITY_ID = "mem_cache_guide_identity_";
    public static GuideXFiveUseIdentityLogic getInstance() {
        return instance;
    }
    
    public static final int IDENTITY_JUNIORHIGHSCHOOL = 11;
    public static final int IDENTITY_SENIORHIGHSCHOOL = 12;
    public static final int IDENTITY_TECHNOLOGYSCHOOL = 13;
    
	public static final int IDENTITY_UNIVERSITY = 20;

	public static final int IDENTITY_WORK = 30;
	
	public static final int IDENTITY_NONE = 90;
	
    
    public int getIdentity(final int userId){

		int identity = 0;
		// 先从mem cache中取
		String key = MEM_CACHE_GUIDE_IDENTITY_ID + userId;
		Object obj = mem.get(key);
		try {
			identity = ((Integer) obj).intValue();
		} catch (NullPointerException e) {
			GuideLogger.printLog("exception accured - "+identity);
		}
		// 如果取不到，从库中取，并把结果放到mem cache中
		if (obj == null) {
			GuideLogger.printLog("obj is null");
			try {
				identity = GuideUserIdentityDAO.getInstance().getIdentity(userId);
				GuideLogger.printLog("from db :"+identity);
			} catch (Exception e) {
				GuideLogger.printLog("db exception accurd:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			mem.set(key, Integer.valueOf(identity), (int) MemCacheKeys.minute * 30);
		}
		return identity;

    }

    
    public int saveIdentity(final int userId, final int identity) {
    	GuideLogger.printLog("host:" + userId + " - identity:" + identity);
		String key = MEM_CACHE_GUIDE_IDENTITY_ID + userId;
		GuideLogger.printLog("host:" + userId + " - " + key);
		try {
			mem.set(key, Integer.valueOf(identity),	(int) MemCacheKeys.minute * 10);
		} catch (Exception e) {
			GuideLogger.printLog("save err host:" + userId + " identity=["	+ identity + "]" + e.toString(), GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		GuideLogger.printLog("save host:" + userId + " identity=[" + identity + "]");
		Runnable task_save = new Runnable() {
			public void run() {
				try {
					GuideUserIdentityDAO.getInstance().saveIdentity(userId, identity);
					GuideLogger.printLog("save host:" + userId + " identity=[" + identity + "]");
				} catch (SQLException e) {
					GuideLogger.printLog("save err host:" + userId + " identity=["	+ identity + "]" + e.toString(), GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(task_save);
		return 1;
    }
    

}
