package com.renren.xoa2.backdoor.test;

import com.renren.xoa2.backdoor.AbstractBackdoorService;
import com.renren.xoa2.client.ServiceFactory;


public class TestServiceImpl extends AbstractBackdoorService   implements ITestService {
	
    @Override
    public void ping(int timeout) {
    	try {
    		if (timeout > 0) {
    			System.out.println("backdoor test time out " + timeout);
    			Thread.sleep(timeout);
    		} 
    		
    		if (timeout < 0) {
    			// 这里尝试抛出一个异常
    			int a = 1/0;
    			System.out.println(a);
    		}
    		
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
    }

    public static void main(String[] args) {
 
    	//System.setProperty("xoa2.hosts.test.backdoor.xoa.renren.com", "127.0.0.1:9091");
        //System.setProperty("xoa2.hosts.twitter.demo.xoa.renren.com", "127.0.0.1:9090");
        
        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);

			try {
				//TestService.ping(10);
				System.out.println(TestService.getName());
				//System.out.println("service alive time is " + TestService.aliveSince());
				TestService.shutdown();
				//Thread.sleep(200);
				//Thread.sleep(10000);
				int i=0;
				while(i++<10)
	                System.out.println(TestService.getName());
				
//				HashMap<String, Long> tab =(HashMap<String, Long>)TestService.getCounters();
//				Iterator<String> ite=tab.keySet().iterator(); 
//	            while (ite.hasNext()) {    
//	             Object key = ite.next();    
//	             System.out.println("service method  : " + key + " use " + tab.get(key) + " times!");    
//	            }             
			} catch (Exception e1) {
				System.out.println("----------|||1");
				e1.printStackTrace();
			}
//			int i=0;
//            while(i++<200)
//			try {
//			    System.out.println(TestService.getName());
//			} catch (Exception e1) {
//                System.out.println("----------|||2");
//                e1.printStackTrace();
//            }
			
        	try {
				Thread.sleep(0);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
	       System.out.println("post ok");
        
    }

}
