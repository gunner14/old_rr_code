package com.renren.xoa2.demo.twitter;


import com.renren.xoa2.client.ServiceFactory;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 29, 2011 6:42:15 PM
 */
public class ClientExampleMain {

    
    public static void main(String[] args) {
        int length = 1;
        if (args.length > 0) {
        	length = Integer.parseInt(args[0]);
        }
        System.setProperty("xoa2.hosts.twitter.demo.xoa.renren.com", "127.0.0.1:9090");
        
        
			
			try {
				ITwitterService twitterService = ServiceFactory.getService(ITwitterService.class, 100);
				String txt = "Hello XOA 2.0.";
				while (txt.length() < length) {
					txt += "dummy.";
				}
				Tweet tweet = new Tweet();
				tweet.setUserName("Jeo");
				tweet.setUserId(12345);
				tweet.setText(txt);
				twitterService.postTweet(tweet);
				
				TweetSearchResult res = twitterService.searchTweets("hello");
				System.out.println(res);
			} catch (Exception e1) {
				System.out.println("----------|||");
				e1.printStackTrace();
			}
			
        	try {
				Thread.sleep(0);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
	       System.out.println("post ok");
	       System.exit(-1);
        // }
    }
}
