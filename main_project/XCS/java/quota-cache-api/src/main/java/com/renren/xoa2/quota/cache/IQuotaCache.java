package com.renren.xoa2.quota.cache;

import java.util.Set;

import com.renren.xoa2.annotation.XoaService;

/**
 *
 * @author Xue Wenxin Nov 10, 2012 5:05:07 PM
 */
@XoaService("quota.cache.xoa.renren.com")
public interface IQuotaCache extends QuotaCache.Iface {
	
	/**删除某个key**/
	public boolean remove(String key);
	
	/**添加key到cache中，并赋值为value，过期时间为expireTime**/
	public void setLong(String key, long value, int expireTime) throws QuotaCacheException;
	
	/**获取某个key对应的计数值**/
	public long getLong(String key) throws QuotaCacheException;
	
	/**增加某个key对应的计数值增加value**/
	public long incLong(String key, long value) throws QuotaCacheException;
	
	/**取某个key对应的ip集合**/
	public Set<String> getIps(String key);

	/**将ip添加到key对应的键值对中*/
	public void putIp(String key,String ip, int expireTime) throws QuotaCacheException;
	
	/**取某个key对应到single quota使用情况**/
	public long getSingleQuotaUsed(String level, String service,String version, String clientId, String ip) throws QuotaCacheException;
	
	/**取某个key对应到total quota使用情况**/
	public long getTotalQuotaUsed(String level, String service,String version, String clientId) throws QuotaCacheException;
}
