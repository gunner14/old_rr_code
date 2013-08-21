package com.renren.xoa2.demo.twitter;

import com.renren.xoa2.client.ServiceFactory;

public class ServiceFactoryTest {

	/**
	 * @param args
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws InterruptedException {
        ITwitterService twitterService = ServiceFactory.getService(ITwitterService.class, 1000);
        
        String txt = "Hello XOA 2.0.";
        Tweet tweet = new Tweet();
        tweet.setUserName("Jeo");
        tweet.setUserId(12345);
        tweet.setText(txt);
        int i = 0;
        while (true) {
        	try {
            	twitterService.postTweet(tweet);
			}  catch (Exception e) {
				e.printStackTrace();
			} finally {
	     	    System.out.println("Sleeping ... zzz ...");
	     	    if (++i > 10) {
	     	    	// Thread.sleep(10);
	     	    }
			}
     	    // break;
        }
	}
}
