package com.xiaonei.reg.guide.flows.xfive.logics;

import java.util.List;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;


public class GuideXFiveCommonCacheLogic {
	private static final String MEM_CACHE_GUIDE_REQUEST_FRIEND_CONFIRM_LIST = "mem_cache_guide_confirm_list_";
	
	private static final int MINITS = 5;
	
	private static final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
	
	static class SingletonHolder {
		static GuideXFiveCommonCacheLogic instance = new GuideXFiveCommonCacheLogic();
	}

	public static GuideXFiveCommonCacheLogic getInstance() {
		return SingletonHolder.instance;
	}
	
	
	public void saveConfirmList(User host, List<Integer> info) {
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return;
		}
		if(info == null){
			GuideLogger.printLog("info null! warning host:"+host.getId(),GuideLogger.WARN);
			return;
		}
		
		try {
			String key = MEM_CACHE_GUIDE_REQUEST_FRIEND_CONFIRM_LIST + host.getId();
			mem.set(key, info,(int) MemCacheKeys.minute * MINITS);
			GuideLogger.printLog("Mem saved host:"+host.getId()+" "+info.toString());
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		
	}
	
	@SuppressWarnings("unchecked")
	public List<Integer> loadConfirmList(User host){
		if(host == null ){
			GuideLogger.printLog("host null! warning",GuideLogger.WARN);
			return null;
		}
		List<Integer> info = null;
		try {
			String key = MEM_CACHE_GUIDE_REQUEST_FRIEND_CONFIRM_LIST + host.getId();
			Object obj =  mem.get(key);
			if(obj!=null){
				info = (List<Integer>)obj;
				GuideLogger.printLog("Mem geted host:"+host.getId()+" list:"+info.size());
			}
			else{
				GuideLogger.printLog("Mem get null host:"+host.getId()+" stage:"+host.getStage());
			}
		} catch (Exception e) {
			GuideLogger.printLog("Mem error host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return info;
	}



	



}
