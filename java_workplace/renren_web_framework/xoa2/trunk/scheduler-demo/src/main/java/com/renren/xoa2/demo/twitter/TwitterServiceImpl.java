package com.renren.xoa2.demo.twitter;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 28, 2011 7:22:27 PM
 */
public class TwitterServiceImpl implements ITwitterService{

    private List<Tweet> tweets = Collections.synchronizedList(new ArrayList<Tweet>());
    
    final int THRESHOLD = 100000;
    
    long zipNumber;
    
    @Override
    public void ping(int timeout) {
    	try {
    		if (timeout > 0) {
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

    @Override
    public boolean postTweet(Tweet tweet) {
    	synchronized (tweets) {
        	tweets.add(tweet);
            if (tweets.size() > THRESHOLD) {
            	tweets.remove(0);
            }
		}

        return true;
    }

    @Override
    public TweetSearchResult searchTweets(String query) {
        TweetSearchResult res = new TweetSearchResult();
        if (query.equals("latest")) {
            if (tweets.size() > 0) {
                res.addToTweets(tweets.get(tweets.size() - 1));
            }
        } else {
            for (Tweet t : tweets) {
                res.addToTweets(t);
            }
        }
        return res;
    }

    @Override
    public void zip() {
    	if ((zipNumber % 10000) == 0) {
    		++zipNumber;
    		System.out.println("zip()");
    	}
    }

}
