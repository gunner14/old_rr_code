package com.renren.xoa2.demo.twitter;

import com.renren.xoa2.annotation.XoaService;

@XoaService("twitter.demo.xoa.renren.com")
public interface ITwitterService extends TwitterService.Iface {

    public void ping(int timeout);

    public boolean postTweet(Tweet tweet);

    public TweetSearchResult searchTweets(String query) throws InvalidException;

    public void zip();
    
}
