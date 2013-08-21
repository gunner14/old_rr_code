package com.renren.sns.minisite.feed.xoa.services.fetch;

import java.util.List;


public interface FeedsFetchService {

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Jul 13, 2011 7:43:43 PM
     * 
     * @param userId
     * @param page from 0
     * @return
     */
    public List<Long> getFeeds(int userId, int page);
}
