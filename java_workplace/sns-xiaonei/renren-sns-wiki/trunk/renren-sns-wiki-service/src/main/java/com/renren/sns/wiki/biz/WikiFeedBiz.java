package com.renren.sns.wiki.biz;

import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.feed.Feed2MapUtils;
import com.renren.sns.wiki.delegator.xce.FeedLogicDelegator;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-4
 * 
 */
@Service
public class WikiFeedBiz {

    @Autowired
    private FeedLogicDelegator feedLogicDelegator;

    /**
     * 根据feedId数组获取FeedSet
     * 
     * @param currentName
     * @param currentID
     * @param feedIds
     * @return
     * @throws Exception
     */
    public FeedSet getFeedSetByIds(String currentName, int currentID, long[] feedIds)
            throws Exception {
        FeedSet feedSet = feedLogicDelegator.getFeedSetByIds(currentName, currentID, feedIds);
        return feedSet;
    }

    /**
     * 根据wikiId获取最新的新鲜事
     * 
     * @param wikiId
     * @param begin
     * @param limit
     * @param types
     * @return
     */
    public long[] getFeedIdByTypesFromSocialWikiLatestByWikiId(int wikiId, int begin, int limit,
            int... types) {
        return feedLogicDelegator.getFeedIdByTypesFromSocialWikiLatestByWikiId(wikiId, begin,
                limit, types);
    }

    public long[] getFeedIdByTypesFromSocialWikiHotByWikiId(int wikiId, int begin, int limit,
            int[] types) {
        return feedLogicDelegator.getFeedIdByTypesFromSocialWikiHotByWikiId(wikiId, begin, limit,
                types);
    }

    public long[] getFeedidByTypesFromSocialWikiByUserid(int[] userIds, int wikiId, int begin,
            int limit, int... types) {
        long[] feedIds = feedLogicDelegator.getFeedidByTypesFromSocialWikiByUserid(userIds, wikiId,
                begin, limit, types);
        return feedIds;
    }

    /**
     * 将feed转化为jsonObject形式
     * 
     * @param feed
     * @return
     */
    public Map<String, Object> covert2Map(Feed feed) {
        return Feed2MapUtils.covert(feed);
    }

}
