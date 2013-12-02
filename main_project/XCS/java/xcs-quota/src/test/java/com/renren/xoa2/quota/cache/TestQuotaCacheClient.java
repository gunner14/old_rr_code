package com.renren.xoa2.quota.cache;

import org.junit.Test;

import com.renren.xoa2.client.DefaultServiceFactory;
import com.renren.xoa2.client.IServiceFactory;

/**
 *
 * @author Xue Wenxin Sep 19, 2012 4:54:22 PM
 */
public class TestQuotaCacheClient {
	private IServiceFactory factory = new DefaultServiceFactory();
	private IQuotaCache cache = factory.getService(IQuotaCache.class);
	
	public String key = "h_twitter.demo.xoa.renren.com_1_api";
	public String key1="SEC_test-quota-counter";
	
	@Test()
	public void setLong(){
		try {
			cache.setLong(key1, 1l, QuotaCacheConstants.EXPIRE_MINUTE);
		} catch (QuotaCacheException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	@Test
	public void getLong(){
		try {
			System.out.println("key:"+key+",value:"+cache.getLong(key1));
		} catch (QuotaCacheException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	@Test
	public void incLong(){
		try {
			System.out.println("key:"+key1+",value:"+cache.incLong(key1,100));
		} catch (QuotaCacheException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	@Test
	public void testIpCache() throws QuotaCacheException{
		String[] ip = {"10.0.0.4","10.0.0.5","10.0.0.6"};
		for(int i = 0;i<3;i++){
			cache.putIp(key, ip[i],  30*QuotaCacheConstants.EXPIRE_MINUTE);
			System.out.println(cache.getIps(key));
		}
	}
	@Test
	public void testIncLong() throws InterruptedException, QuotaCacheException{
		cache.setLong(key1, 0, QuotaCacheConstants.EXPIRE_SECOND*3);
		for(int i=0;i<100;i++){
			Thread.sleep(100);
			System.out.println(cache.incLong(key1, 1));
		}
	}

}
