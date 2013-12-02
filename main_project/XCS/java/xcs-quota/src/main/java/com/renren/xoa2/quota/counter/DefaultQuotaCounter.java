package com.renren.xoa2.quota.counter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.datacenter.find.bean.QuotaInfo2.QuotaLevel;
import com.renren.xoa2.quota.Counter;
import com.renren.xoa2.quota.cache.IQuotaCache;
import com.renren.xoa2.quota.cache.QuotaCacheConstants;

/**
 * quota counter的默认实现(累积到一定的阈值(1000)再写update cache)
 * @author Xue Wenxin Sep 18, 2012 3:18:20 PM
 */
public class DefaultQuotaCounter implements QuotaCounter {
	
	private Log logger = LogFactory.getLog(DefaultQuotaCounter.class);
	
	private IQuotaCache quotaCache;
	
	private Counter counter = new Counter();
	
	private int threshold = 1000;
	
	public DefaultQuotaCounter(IQuotaCache cache){
		quotaCache = cache;
	}

	@Override
	public long increase(QuotaLevel level, String key) {
		String counterKey = level+"_"+key;
		switch (level) {
		case HOUR:
			return increase(counterKey,QuotaCacheConstants.EXPIRE_HOUR);
		case MIN:
			return increase(counterKey,QuotaCacheConstants.EXPIRE_MINUTE);
		case SEC:
			return increase(counterKey,QuotaCacheConstants.EXPIRE_SECOND);
		default:
			return 0;
		}
	}
	//通用cache解决方案实施后可以采用这个方法
	private long increase(String key, int expireTime){
		long current = 0;
		try {
			current = counter.increaseCount(key);
			if(current % threshold == 1){
				if(quotaCache.getLong(key) == QuotaCacheConstants.ERROR_COUNT){
					quotaCache.setLong(key, 1, expireTime);
					counter.setCount(key, 1);
					current = 1;
				}else{
					if(current > 1){
						current = quotaCache.incLong(key, threshold);
						if(current == QuotaCacheConstants.ERROR_COUNT){
							quotaCache.setLong(key, 1, expireTime);
							counter.setCount(key, 1);
							current = 1;
						}else{
							counter.setCount(key, (int) current);
						}
					}
				}
			}
		} catch (Exception e) {
			logger.error(e.getMessage());
			return 0; //屏蔽计数相关的任何异常，保证不影响client端调用
		}
		
		return current;
	}
	
	@Override
	public long getCurrent(QuotaLevel level, String key) {
		return counter.getCount(level+"_"+key);
	}

}
