package com.renren.xoa2.quota;

import org.junit.Test;

import com.renren.xcs.FindServiceRegistryFactory;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xcs.XcsConfigurations;
import com.renren.xoa2.quota.checker.QuotaChecker;
import com.renren.xoa2.quota.counter.TestQuotaCounter;

/**
 *
 * @author Xue Wenxin Sep 19, 2012 4:07:32 PM
 */
public class TestQuotaCheckerFactory {
	@Test
	public void test(){
		QuotaCheckerFactory ff = new QuotaCheckerFactory();
		FindServiceRegistryFactory f = new FindServiceRegistryFactory();
		ServiceRegistryAccessor accessor = null;
		
		accessor = f.getServiceRegistryAccessor(XcsConfigurations.getConfiguration(),"twitter.demo.xoa.renren.com","1");
		QuotaChecker checker = ff.getQuotaChecker(accessor,XcsConfigurations.getConfiguration(),TestQuotaCounter.factory);
		System.out.println(checker);
		
		
	}
}
