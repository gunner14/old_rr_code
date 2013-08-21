package com.renren.sns.wiki.biz.feed;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.dao.feed.HotFeedDAO;
import com.renren.sns.wiki.delegator.xce.FeedLogicDelegator;
import com.renren.sns.wiki.model.hotfeed.HotFeedRecord;

/**
 * 
 * 该服务用来操作Feed
 * 
 * @author yi.li@renren-inc.com since 2012-7-5
 * 
 */
@Service
public class FeedOperationBiz extends BaseBiz{

    @Autowired
    private HotFeedDAO hotFeedDAO;

    @Autowired
    private FeedLogicDelegator feedLogicDelegator;

    /**
     * 修改热门feed的权重
     * 
     * @param wikiId wiki id
     * @param feedId 新鲜事id
     * @param weight 权重
     */
    public void updateHotFeedWeight(int wikiId, long feedId, int type, int weight) {
        HotFeedRecord record = new HotFeedRecord();
        record.setFeedId(feedId);
        record.setFeedType(type);
        record.setWikiId(wikiId);
        record.setPosition(weight);

        hotFeedDAO.updateWikiHotFeed(wikiId, record);
    }

    /**
     * 获取第一个最热新鲜事
     * 
     * @param wikiId
     * @return HotFeedRecord
     */
    public HotFeedRecord getTopHotFeed(int wikiId) {
        List<HotFeedRecord> result = hotFeedDAO.getWikiHotFeedByWeight(wikiId, 0, 1);
        if (null == result || result.size() == 0) {
            return null;
        }

        return result.get(0);
    }

    /**
     * 删除最热新鲜事
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    public void removeHotFeed(int wikiId, long feedId) {
        hotFeedDAO.deleteWikiHotFeedRecord(wikiId, feedId);
        feedLogicDelegator.removeFromSocialWikiHotByWikiId(wikiId, feedId);
    }

    /**
     * 批量删除feed
     * 
     * @param wikiId wiki id
     * @param feedIds feedid列表
     */
    public void removeHotFeeds(int wikiId, List<Long> feedIds) {
        hotFeedDAO.deleteWikiHotFeedRecords(wikiId, feedIds);
        feedLogicDelegator.reloadFromSocialWikiHotByWikiId(wikiId);
    }

    /**
     * 删除最新新鲜事
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    public void removeNewFeed(int wikiId, long feedId) {
        feedLogicDelegator.removeFromSocialWikiNewsByWikiId(wikiId, feedId);
    }

    /**
     * 根据Wiki Id重新加载最热新鲜事
     * 
     * @param wikiId wiki id
     */
    public void reloadHotFeedByWiki(int wikiId) {
        feedLogicDelegator.reloadFromSocialWikiHotByWikiId(wikiId);
    }
}
