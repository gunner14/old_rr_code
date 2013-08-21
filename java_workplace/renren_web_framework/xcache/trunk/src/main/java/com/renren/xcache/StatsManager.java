package com.renren.xcache;

import java.util.Map;

/**
 * 
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-10-8 下午12:26:53
 */
public interface StatsManager {
	
	public abstract Map<String, Map<String, String>> stats();
    
    public abstract Map<String, Map<String, String>> statsItems();
    
    public abstract Map<String, Map<String, String>> statsCacheDump(int slabNumber, int limit);
    
    public abstract Map<String, Map<String, String>> statsSlabs();
}
