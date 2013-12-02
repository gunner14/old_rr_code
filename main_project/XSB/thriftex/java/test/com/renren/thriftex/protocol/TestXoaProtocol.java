package com.renren.thriftex.protocol;

import java.util.ArrayList;
import java.util.List;

import org.apache.thrift.TException;

import com.renren.xoa2.client.ServiceFactory;
import com.renren.xoa2.demo.twitter.ITwitterService;
import com.renren.xoa2.demo.twitter.Tweet;

public class TestXoaProtocol {
	
	static class LoadRunner implements Runnable {

        @Override
        public void run() {
        	ITwitterService twitterService = ServiceFactory.getService(ITwitterService.class, 500);
            String txt = "Hello XOA 2.0.";
            while (txt.length() < 1) {
            	txt += "dummy.";
            }
            Tweet tweet = new Tweet();
            tweet.setUserName("Jeo");
            tweet.setUserId(12345);
            tweet.setText(txt);
            int i = 0;
             while (i++ < 10) {
    			 //twitterService.postTweet(tweet);
    			
    			try {
    				twitterService.ping(15);
    				twitterService.searchTweets("hello");
    			} catch (Exception e1) {
    				System.out.println("----------|||");
    				e1.printStackTrace();
    			}
    			
             }
        }
	}
        
        public static class ExitHandler implements Runnable {  
    		List<Thread> threadList;
    		
    		public ExitHandler(List<Thread> list) {  
    			threadList = list;
    		}  
    		
    		@SuppressWarnings("deprecation")
    		@Override
    		public void run() {  
    			System.out.println("Terminating process... kill all thread!");
    			for (Thread thread: threadList) {
    				thread.stop(new Exception("kill!"));
    				try {
    					thread.join();
    				} catch (InterruptedException e) {
    					// TODO Auto-generated catch block
    					e.printStackTrace();
    				}
    			} 
    		}  
    	}
        
        
	public static void main(String[] args) {
		List<Thread> threadList = new ArrayList<Thread>();
		 System.setProperty("xoa2.hosts.twitter.demo.xoa.renren.com", "127.0.0.1:9090");
		
		for( int i=0; i<10; i++){
    		Thread thread = new Thread( new LoadRunner(), "loadRunner" + String.valueOf(i));
    		thread.start();
    		threadList.add(thread);
        }
		
      Thread exitThread = new Thread(new TestXoaProtocol.ExitHandler(threadList));
    	
    	Runtime.getRuntime().addShutdownHook(exitThread);          
        
    	try {
        	for (Thread thread: threadList) {
                thread.join();        		
        	}
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    	
    	try {
    		XoaParseProtocol testParseProtocol = new XoaParseProtocol("../xoa2-demo/BinaryFile");  //读二进制文件里的信息
    		testParseProtocol.parseMessage("../xoa2-demo/ParseBinaryFile");   //解析二进制的文件后存到的文件
			Thread.sleep(0);
    	} catch (TException e) {
    		e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
    	
    	
	}
}

