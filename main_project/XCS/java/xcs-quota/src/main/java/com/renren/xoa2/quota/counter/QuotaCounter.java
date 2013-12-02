package com.renren.xoa2.quota.counter;

import com.renren.ad.datacenter.find.bean.QuotaInfo2.QuotaLevel;

/**
 * quota counter 接口抽象
 * @author Xue Wenxin Sep 17, 2012 8:25:50 PM
 */
public interface QuotaCounter {
	
	
	/**counter + 1，并返回+1后结果*/
	public long increase(QuotaLevel level, String key);
	
	/**取level + key 相关到 当前计数值*/
	public long getCurrent(QuotaLevel level, String key);
//	/**total counter + 1，并返回+1后结果*/
//	public long increaseTotal(QuotaLevel level, String key);
	
	
}
