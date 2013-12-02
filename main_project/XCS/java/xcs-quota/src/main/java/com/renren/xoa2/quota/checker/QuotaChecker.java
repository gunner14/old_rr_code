package com.renren.xoa2.quota.checker;



/**
 * 配额检查器接口抽象
 * @author Xue Wenxin Aug 14, 2012 4:54:56 PM
 */
public interface QuotaChecker {
	
	/**
	 * 判断某client(支持单台机器，也支持所有机器)对某service是否有配额
	 * @return 
	 */
	public boolean hasQuota();

}
