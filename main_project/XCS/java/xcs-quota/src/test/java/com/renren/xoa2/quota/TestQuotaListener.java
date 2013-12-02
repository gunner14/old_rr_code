package com.renren.xoa2.quota;

import org.junit.Test;

import com.renren.xcs.Configuration;
import com.renren.xcs.FindServiceRegistryFactory;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xcs.XcsConfigurations;
import com.renren.xoa2.quota.checker.DefaultQuotaChecker;
import com.renren.xoa2.quota.checker.QuotaChecker;
import com.renren.xoa2.quota.counter.TestQuotaCounter;

/**
 * QuotaListener test case<br>
 * 测试一个consumer注册不同的listener是否有效
 * @author Xue Wenxin Sep 5, 2012 12:24:03 PM
 */
public class TestQuotaListener {
	FindServiceRegistryFactory f = new FindServiceRegistryFactory();
	QuotaCheckerFactory ff = new QuotaCheckerFactory();
	public ServiceRegistryAccessor getAccessor(String service, String version){
		ServiceRegistryAccessor accessor = null;
		accessor = f.getServiceRegistryAccessor(XcsConfigurations.getConfiguration(),"twitter.demo.xoa.renren.com","2");
		
		return accessor;
	}
	Configuration conf = XcsConfigurations.getConfiguration();
	ServiceRegistryAccessor accessor1 = getAccessor("twitter.demo.xoa.renren.com","2");
	ServiceRegistryAccessor accessor2 = getAccessor("comments.status.ugc.tech.xoa","2");

	@Test
	public void testListener(){
		QuotaChecker check = ff.getQuotaChecker(accessor1,conf,TestQuotaCounter.factory);
		System.out.println(((DefaultQuotaChecker)check).getQuotaInfo());
		//TODO:运维工具，手动修改zk中“twitter.demo.xoa.renren.com,1"下的quota信息
		try {
			Thread.sleep(60*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		QuotaChecker check1 = ff.getQuotaChecker(accessor2,conf,TestQuotaCounter.factory);
		System.out.println(((DefaultQuotaChecker)check1).getQuotaInfo());
		//TODO:运维工具，手动修改zk中“twitter.demo.xoa.renren.com,1"下的quota信息
		try {
			Thread.sleep(60*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		
		System.out.println(((DefaultQuotaChecker)check).getQuotaInfo());
	}
}
/* 测试结果
QuotaInfo2 [clientId=api, singleMap={HOUR=100}, totalMap={HOUR=1200}]
DataChange:QuotaInfo2 [clientId=api, singleMap={HOUR=110}, totalMap={HOUR=1500}]
QuotaInfo2 [clientId=api, singleMap={HOUR=100, MIN=10}, totalMap={HOUR=1000, MIN=60}]
DataChange:QuotaInfo2 [clientId=api, singleMap={HOUR=110}, totalMap={HOUR=1500}]
DataChange:QuotaInfo2 [clientId=api, singleMap={HOUR=110}, totalMap={HOUR=15000}]
QuotaInfo2 [clientId=api, singleMap={HOUR=110}, totalMap={HOUR=15000}]
*/
