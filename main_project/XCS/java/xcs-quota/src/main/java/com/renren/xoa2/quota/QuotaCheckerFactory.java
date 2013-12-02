package com.renren.xoa2.quota;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xcs.Configuration;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xoa2.client.IServiceFactory;
import com.renren.xoa2.quota.cache.IQuotaCache;
import com.renren.xoa2.quota.checker.DefaultQuotaChecker;
import com.renren.xoa2.quota.checker.QuotaChecker;
import com.renren.xoa2.quota.processor.AlarmExceedProcessor;
import com.renren.xoa2.quota.processor.ExceedProcessor;


/**
 * QuotaChecker的工厂类
 * @author Xue Wenxin Aug 14, 2012 8:57:47 PM
 */
public class QuotaCheckerFactory {
	
	private Log logger = LogFactory.getLog(QuotaCheckerFactory.class);
	private ConcurrentMap<String, QuotaChecker> quotaCheckerCache = new ConcurrentHashMap<String, QuotaChecker>();
	
	public QuotaChecker getQuotaChecker(ServiceRegistryAccessor accessor, Configuration config, IServiceFactory factory){
		IQuotaCache cache = factory.getService(IQuotaCache.class);
		String key = getCheckerKey(accessor.getService(), accessor.getVersion());
		QuotaChecker quotaChecker = quotaCheckerCache.get(key);
		if(quotaChecker == null){
			synchronized (this){
				quotaChecker = quotaCheckerCache.get(key);
				if(quotaChecker == null){
					ExceedProcessor processor = new AlarmExceedProcessor(accessor,config);
					quotaChecker = new DefaultQuotaChecker(accessor, config, processor, cache);
					quotaCheckerCache.put(key, quotaChecker);
				}
			}
			
		}
		return quotaChecker;
	}
	

	
	private String getCheckerKey(String serviceId, int version) {
		return serviceId + "_" + version;
	}

	public int getSize(){
		return quotaCheckerCache.size();
	}
	
}
