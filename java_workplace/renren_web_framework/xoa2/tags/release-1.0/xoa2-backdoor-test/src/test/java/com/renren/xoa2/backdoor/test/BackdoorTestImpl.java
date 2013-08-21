package com.renren.xoa2.backdoor.test;

import java.util.List;

import org.junit.Test;

import com.renren.xoa2.client.ServiceFactory;
import com.renren.xoa2.demo.twitter.ITwitterService;

public class BackdoorTestImpl {
      
//    @Test
//    public void TestBackdoorMilitiaService() {
//        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);
//
//        try {
//            //System.out.println("Service id  is " + TestService.getName());
//            //System.out.println("service alive time is " + TestService.aliveSince());
//            //TestService.getStatus();
//            TestService.startMilitiaService("test.backdoor.xoa.renren.com");
//            //TestService.ping(10);  
//            Thread.sleep(200);
//        } catch (Exception e1) {
//            System.out.println("----------|||1");
//            e1.printStackTrace();
//        }
//        
//        
//        ITwitterService TwitterService = ServiceFactory.getService(ITwitterService.class, 50000);
//        try {
//            for(int i = 0; i < 10; i++) {
//                System.out.println("Twitter Service ping is 10");
//                TwitterService.ping(10);
//            }
//            Thread.sleep(0);
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//    }
//    
//    static class LoadRunner implements Runnable {
//    @Override
//    public void run() {
//        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);
//        
//        try {
//            int i = 0;
//            for(i = 0; i < 100; i++) {
//                TestService.ping(20);
//            }     
//            } catch (Exception e1) {
//                System.out.println("----------|||");
//                e1.printStackTrace();
//            }   
//        } 
//    }
//    
//    public static class ExitHandler implements Runnable {  
//        List<Thread> threadList;
//        
//        public ExitHandler(List<Thread> list) {  
//            threadList = list;
//        }  
//        
//        @SuppressWarnings("deprecation")
//        @Override
//        public void run() {  
//            System.out.println("Terminating process... kill all thread!");
//            for (Thread thread: threadList) {
//                thread.stop(new Exception("kill!"));
//                try {
//                    thread.join();
//                } catch (InterruptedException e) {
//                    // TODO Auto-generated catch block
//                    e.printStackTrace();
//                }
//            } 
//        }  
//    }
//    
//    @Test
//    public void TestBackdoorThreadStatistics() {
//        //for(int index=0;index<10;index++){
//        List<Thread> threadList = new ArrayList<Thread>();
//       
//        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);
//        for( int i=0; i<10; i++){
//           Thread thread = new Thread( new LoadRunner(), "loadRunner" + String.valueOf(i));
//           thread.start();
//           threadList.add(thread);
//        }
//       
//        Thread exitThread = new Thread(new BackdoorTestImpl.ExitHandler(threadList));
//       
//        Runtime.getRuntime().addShutdownHook(exitThread);          
//       
//        try {
//            for (Thread thread: threadList) {
//               thread.join();              
//            }
//            
//          System.out.println(TestService.getCounter("ping"));
//          System.out.println(TestService.getMean("ping"));
//          
//          System.out.println(TestService.getMax("ping", 60)); 
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//        //}
//    }
    
    @Test
    public void TestBackdoorStatistics() {
        
        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);

        try {
            System.out.println("TestServiceImpl is " + TestService.getName());
            System.out.println("service alive time is " + TestService.aliveSince());
            
            int i = 0;
            for(i = 0; i < 10000; i++) {
                TestService.ping(20);
            }
            //Thread.sleep(3000);
            
            System.out.println(TestService.getCounter("ping"));
            System.out.println(TestService.getMean("ping"));
            
            System.out.println(TestService.getMax("ping", 60));
                        
        } catch (Exception e1) {
            System.out.println("----------|||1");
            e1.printStackTrace();
        }

        
        try {
            Thread.sleep(0);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
//    private int getModule(int time) {
//        int index;
//        if(time <= 0) {
//            index = time + 86400;
//        } else {
//            index = time;
//        }
//        return index % 86400;
//    }
//    
//    @Test
//    public void TestBackdoorCalcSecond() {
//        int time = (int)(((System.currentTimeMillis()+(8*3600*1000))%(1000*24*60*60))/1000);
//        SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
//        System.out.println(sdf.format(new java.util.Date())); 
//        long l = System.currentTimeMillis();
//        Date date = new Date(l);
//        Calendar cal = Calendar.getInstance();
//        TimeZone timeZone = cal.getTimeZone();//得到和UTC时区的偏移值
//        System.out.println(timeZone.getRawOffset());
//        cal.setTime(date);
//        System.out.println(cal.get(Calendar.AM_PM)*12 + cal.get(Calendar.HOUR));
//        System.out.println(cal.get(Calendar.MINUTE));
//        System.out.println(cal.get(Calendar.SECOND));
//        System.out.println(time);
//        System.out.println((cal.get(Calendar.AM_PM)*12 + cal.get(Calendar.HOUR))*3600 + cal.get(Calendar.MINUTE)*60 + cal.get(Calendar.SECOND));
//        assertEquals(time, (cal.get(Calendar.AM_PM)*12 + cal.get(Calendar.HOUR))*3600 + cal.get(Calendar.MINUTE)*60 + cal.get(Calendar.SECOND));      
//        
//        
//        assertEquals(86398, getModule(-2));
//        assertEquals(86399, getModule(-1));
//        assertEquals(0, getModule(0));
//        assertEquals(1, getModule(1));
//        assertEquals(0, getModule(86400));
//    }
    
//    @Test
//    public void TestBackdoorReinit() {
//        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);
//
//        try {
//            assertEquals("TestServiceImpl",TestService.getName());
//            System.out.println("service alive time is " + TestService.aliveSince());
//            
//            assertEquals((long)10,TestService.getCounter("ping"));
//            assertEquals("test", TestService.getOption("ping"));
//            
//            TestService.reinitialize();
//            TestService.reinitialize();
//            
//            Thread.sleep(5000);
//            assertEquals(0,TestService.getCounter("ping"));
//            assertEquals(true,TestService.getOptions().isEmpty());
//            
//            System.out.println("service alive time is " + TestService.aliveSince());
//            System.out.println("service alive time is " + TestService.aliveSince());
//            //System.out.println("TestServiceImpl" + TestService.getName());
//            assertEquals("TestServiceImpl",TestService.getName());
//                  
//        } catch (Exception e1) {
//            System.out.println("----------|||1");
//            e1.printStackTrace();
//        }
//
//        
//        try {
//            Thread.sleep(0);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
//    }
    
//    @Test
//    public void TestBackdoorShutdown() {
//        
//        //System.setProperty("xoa2.hosts.test.backdoor.xoa.renren.com", "127.0.0.1:9091");
//        //System.setProperty("xoa2.hosts.twitter.demo.xoa.renren.com", "127.0.0.1:9090");
//        
//        ITestService TestService = ServiceFactory.getService(ITestService.class, 50000);
//
//            try {
//                //TestService.ping(10);
//                assertEquals("test.backdoor.xoa.renren.com",TestService.getName());
//                System.out.println("service alive time is " + TestService.aliveSince());
//                TestService.shutdown();
//                //Thread.sleep(200);
//                //Thread.sleep(10000);
//                int i=0;
//                while(i++<10) {
//                    assertEquals("test.backdoor.xoa.renren.com",TestService.getName());
//                }
//                
////              HashMap<String, Long> tab =(HashMap<String, Long>)TestService.getCounters();
////              Iterator<String> ite=tab.keySet().iterator(); 
////              while (ite.hasNext()) {    
////               Object key = ite.next();    
////               System.out.println("service method  : " + key + " use " + tab.get(key) + " times!");    
////              }             
//            } catch (Exception e1) {
//                System.out.println("----------|||1");
//                e1.printStackTrace();
//            }
////          int i=0;
////            while(i++<200)
////          try {
////              System.out.println(TestService.getName());
////          } catch (Exception e1) {
////                System.out.println("----------|||2");
////                e1.printStackTrace();
////            }
//            
//            try {
//                Thread.sleep(0);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//    }
}
