package com.xiaonei.reg.guide.flows.xfive.logics;

import java.sql.SQLException;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.control.BlockException;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.dao.GuideUserDecisionDAO;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class GuideXFiveUserDecisionLogic {
    private static GuideXFiveUserDecisionLogic instance = new GuideXFiveUserDecisionLogic();
    private static final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
	private static final String MEM_CACHE_GUIDE_DECISION_ID = "mem_cache_guide_decision_";
    public static GuideXFiveUserDecisionLogic getInstance() {
        return instance;
    }
    
	public static final int SOCIAL_GRAPH_NOTSET = 0;
	public static final int SOCIAL_GRAPH_GOOD = 1;
	public static final int SOCIAL_GRAPH_BAD = 2;
    
    public int getDecision(final int userId){
    	boolean ret = false;    	
    	try {
			ret = SnsAdapterFactory.getUserAdapter().get(userId).getUserStateInfo().isSocialGraphGood();
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+userId+"exception accured - err:"+e.toString()+" ",GuideLogger.ERROR);
			System.err.println(" host:"+userId+" e:"+e.toString()+"");
			//e.printStackTrace();
		}

		//2009-12-28 12:19:35 从中间层读数据
		if("".equals("")){
			if(ret == true){
				return SOCIAL_GRAPH_GOOD;
			}
			else{
				return SOCIAL_GRAPH_BAD;
			}
		}

		int dec = 0;
		// 先从mem cache中取
		String key = MEM_CACHE_GUIDE_DECISION_ID + userId;
		Object obj = mem.get(key);
		try {
			dec = ((Integer) obj).intValue();
		} catch (NullPointerException e) {
			GuideLogger.printLog(" host:"+userId+"exception accured - err:"+e.toString()+" "+dec,GuideLogger.ERROR);
		}
		// 如果取不到，从库中取，并把结果放到mem cache中
		if (obj == null) {
			GuideLogger.printLog("obj is null");
			try {
				dec = GuideUserDecisionDAO.getInstance().getDecision(userId);
				GuideLogger.printLog("from db :"+dec);
			} catch (Exception e) {
				GuideLogger.printLog("db exception accurd:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			mem.set(key, Integer.valueOf(dec), (int) MemCacheKeys.minute * 30);
		}
		return dec;

    }
    
    public int saveDecision(final int userId, final int decision) {
    	GuideLogger.printLog("host:" + userId + " - decision:" + decision);
		String key = MEM_CACHE_GUIDE_DECISION_ID + userId;
		GuideLogger.printLog("host:" + userId + " - " + key);
		try {
			mem.set(key, Integer.valueOf(decision),	(int) MemCacheKeys.minute * 30);
		} catch (Exception e) {
			GuideLogger.printLog("save err host:" + userId + " decision=["	+ decision + "]" + e.toString(), GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		//2009-12-28 12:09:46 加上写中间层服务的操作
		try {
			if(decision == SOCIAL_GRAPH_GOOD){
			    SnsAdapterFactory.getUserStateAdapter().setSocialGraphGoodOn(userId);				
			}
			else if(decision == SOCIAL_GRAPH_BAD){
			    SnsAdapterFactory.getUserStateAdapter().setSocialGraphGoodOff(userId);
			}
			else{
				
			}
		} 
		catch (BlockException e){
			GuideLogger.printLog(" host:" + userId + " decision=["	+ decision + "] e:" + e.toString(), GuideLogger.ERROR);
		}
		catch (Exception e) {
			GuideLogger.printLog(" host:" + userId + " decision=["	+ decision + "] e:" + e.toString(), GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		GuideLogger.printLog("save host:" + userId + " decision=[" + decision + "]");
		Runnable task_save = new Runnable() {
			public void run() {
				try {
					GuideUserDecisionDAO.getInstance().saveDecision(userId, decision);
					GuideLogger.printLog("save host:" + userId + " decision=[" + decision + "]");
				} catch (SQLException e) {
					GuideLogger.printLog("save err host:" + userId + " decision=["	+ decision + "]" + e.toString(), GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(task_save);
		return 1;
    }
    

}
