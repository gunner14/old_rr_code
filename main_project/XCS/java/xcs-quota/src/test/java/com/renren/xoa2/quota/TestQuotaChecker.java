package com.renren.xoa2.quota;

import java.util.ArrayList;
import java.util.List;

import com.renren.xcs.Configuration;
import com.renren.xcs.FindServiceRegistryFactory;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xcs.XcsConfigurations;
import com.renren.xoa2.quota.checker.QuotaChecker;
import com.renren.xoa2.quota.counter.TestQuotaCounter;

/**
 * Quota Checker Test Case
 * @author Xue Wenxin Aug 23, 2012 3:55:03 PM
 */
public class TestQuotaChecker {
	private static int threadCount;
	private static int number = 0;
	private static Configuration conf = getConfig();/*XcsConfigurations.getConfiguration()*/;
	private static ServiceRegistryAccessor accessor = getAccessor("twitter.demo.xoa.renren.com","1");
	private static QuotaCheckerFactory factory = new QuotaCheckerFactory();
	
	public static ServiceRegistryAccessor getAccessor(String service, String version) {
		FindServiceRegistryFactory f = new FindServiceRegistryFactory();
		ServiceRegistryAccessor accessor = null;
		accessor = f.getServiceRegistryAccessor(conf, service, version);
		
		return accessor;
	}
	
	public static Configuration getConfig(){
		Configuration conf = null;
		try {
			String xcsFile = "/home/xue/xuespace/dev_1.0_intergratexcs/XCS/java/xcs-core/target/test-classes/xcs-test.properties";
			System.setProperty("xcs.config", xcsFile);
//			System.setProperty("xcs.config", TestQuotaChecker.class
//			        .getResource("xcs-test.properties").toURI().getPath());
		} catch (Exception e) {
			e.printStackTrace();
		}
			conf = XcsConfigurations.getConfiguration();/*new PropertiesFileConfiguration(TestQuotaChecker.class
			        .getResource("xcs-test.properties").toURI().getPath());*/
		return conf;
	}
	
	static class LoadRunner implements Runnable {
        public void run() {
            try {
            	int i = 0;
            	while (++i <= 5) {
            		invoke();
            	}
            } catch (Exception e) {
            	e.printStackTrace();
            } 
        }
        
        private void invoke() {
        	
            QuotaChecker checker = factory.getQuotaChecker(accessor, conf,TestQuotaCounter.factory);
            System.out.println(checker.hashCode()+",factory.size:"+factory.getSize());
//            System.out.println("No."+(number++)+",hasQuota:"+checker.hasQuota());//+","+((DefaultQuotaChecker)checker).getCurrent(QuotaLevel.HOUR));
//            System.out.println("total time count (ms): "+ (System.currentTimeMillis() - start));
        }
           
    }
	
	public static void main(String[] args) {
//		TestQuotaChecker.serviceId = "twitter.demo.xoa.renren.com";
//		TestQuotaChecker.version = "1";
		TestQuotaChecker.threadCount = 15;
//		TestQuotaChecker.checker = factory.getQuotaChecker(serviceId, version);
		List<Thread> threadList = new ArrayList<Thread>();
		for( int i=0; i<threadCount; i++){
    		Thread thread = new Thread( new LoadRunner(), "loadRunner" + String.valueOf(i));
    		thread.start();
    		threadList.add(thread);
		}
		try {
        	for (Thread thread: threadList) {
                thread.join();        		
        	}
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
	}
}
