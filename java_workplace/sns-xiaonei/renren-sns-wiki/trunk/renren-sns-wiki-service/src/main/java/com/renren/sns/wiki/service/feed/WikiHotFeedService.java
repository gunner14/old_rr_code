package com.renren.sns.wiki.service.feed;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.HighLightBiz;
import com.renren.sns.wiki.biz.WikiFeedBiz;
import com.renren.sns.wiki.biz.feed.FeedOperationBiz;
import com.renren.sns.wiki.dao.feed.HotFeedDAO;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.hotfeed.HotFeedRecord;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;

/**
 * 热门新鲜事服务
 * 
 * @TODO 该服务从statistic工程迁移过来后不符合开发规范，之后hotFeed有其他需求时抽取biz层进行重构
 * 
 * @author yi.li@renren-inc.com since 2012-4-24
 * 
 */
@Service
public class WikiHotFeedService {

    private Log logger = LogFactory.getLog(this.getClass());

    @Autowired
    private HotFeedDAO hotFeedDAO;

    @Autowired
    private FeedOperationBiz feedOperationBiz;

    @Autowired
    private WikiFeedBiz wikiFeedBiz;

    @Autowired
    private HighLightBiz highLightBiz;

    /**
     * 获取wiki热门feed信息列表
     * 
     * @param wikiId wiki id
     * @return List<HotFeedRecord> 如果没有获取到记录则返回大小为0的List
     */
    public List<HotFeedRecord> getWikiHotFeedRecord(int wikiId) {
        List<HotFeedRecord> records = hotFeedDAO.getWikiHotFeedRecord(wikiId);
        if (null == records) {
            return Collections.emptyList();
        }
        return records;
    }

    /**
     * 删除某个wiki的热门信息记录
     * 
     * @param wikiId
     * @return true if success, otherwise false
     */
    public boolean clearWikiHotFeedRecord(int wikiId) {
        try {
            hotFeedDAO.clearWikiHotFeedRecord(wikiId);
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
        return true;
    }

    /**
     * 更新某个wiki的热门feed记录
     * 
     * @param records 热门feed记录列表
     * @param wikiId wiki id
     * @return true if success, otherwise false
     */
    public boolean updateHotFeedRecord(List<HotFeedRecord> records, int wikiId) {
        try {
            hotFeedDAO.clearWikiHotFeedRecord(wikiId);
            hotFeedDAO.insertWikiHotFeedRecord(records, wikiId);
            highLightBiz.cleanHightCache(wikiId);
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
        return true;
    }

    /**
     * 添加一条热门新鲜事
     * 
     * @param record 最热新鲜事记录
     * @param wikiId wiki id
     * @see HotFeedRecord
     * @return true if success otherwise false
     */
    public boolean addHotFeedRecord(HotFeedRecord record, int wikiId) {
        try {
            List<HotFeedRecord> records = new ArrayList<HotFeedRecord>();
            records.add(record);
            hotFeedDAO.insertWikiHotFeedRecord(records, wikiId);
            return true;
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
    }

    /**
     * 根据WikiId和FeedId删除热门新鲜事记录
     * 
     * @param wikiId wikiId
     * @param feedId feedId
     * @return true if success otherwise false
     */
    public boolean deleteHotFeedRecord(int wikiId, long feedId) {
        try {
            // TODO 这里不应该直接引用DAO，是因为由statistic引入的，下一次修改适时的重构该方法
            hotFeedDAO.deleteWikiHotFeedRecord(wikiId, feedId);
            feedOperationBiz.removeHotFeed(wikiId, feedId);
            highLightBiz.cleanHightCache(wikiId);
            return true;
        } catch (Exception e) {
            logger.error(String.format(
                    "Hot Feed. remove hot feed record from db error. wikiId-%d feedId-%d", wikiId,
                    feedId), e);
            return false;
        }
    }

    /**
     * 将某个Feed放到最热Feed中
     * 
     * @param wikiId wiki Id
     * @param feedId 新鲜事Id
     */
    public void topFeed(int wikiId, long feedId) {
        try {
            int weight = 1000;
            HotFeedRecord record = feedOperationBiz.getTopHotFeed(wikiId);
            if (record != null) {
                weight = record.getPosition() + 100;
            }

            FeedSet feedset = wikiFeedBiz.getFeedSetByIds("", wikiId, new long[] { feedId });
            if (null == feedset || null == feedset.getData() || feedset.getData().length != 1) {
                logger.warn("Top Feed Operation: Feed not exists wikiId " + wikiId + ", feedId "
                        + feedId);
                throw new WikiException(WikiExceptionType.FEED_NOT_EXISTS, "wikiId: " + wikiId
                        + ", feedId: " + feedId);
            }
            Feed feed = feedset.getData()[0];
            int stype = feed.getFeedType();

            feedOperationBiz.updateHotFeedWeight(wikiId, feedId, stype, weight);
            feedOperationBiz.reloadHotFeedByWiki(wikiId);

            highLightBiz.cleanHightCache(wikiId);

            //            这里感觉有点生搬硬套了，要考虑合适的场景
            //            HotFeedUpdatedEvent event = new HotFeedUpdatedEvent();
            //            event.setWikiId(wikiId);
            //            EventUtil.getEventManager().dispatchEvent(event);
        } catch (WikiException e) {
            throw e;
        } catch (Exception e) {
            logger.error(String.format("Put feed to top hot feed error, wikiId-%d feedId-%d",
                    wikiId, feedId), e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 重新加载某个Wiki的最热新鲜事
     * 
     * @param wikiId
     */
    public void reloadWikiHotFeed(int wikiId) {
        try {
            feedOperationBiz.reloadHotFeedByWiki(wikiId);
        } catch (Exception e) {
            logger.error("Reload wiki hot feed error wiki id: " + wikiId, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 删除某一条最新新鲜事
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    public void deleteNewsFeed(int wikiId, long feedId) {
        try {
            feedOperationBiz.removeNewFeed(wikiId, feedId);
        } catch (Exception e) {
            logger.error("Remove news feed error wiki id: " + wikiId + ", feed id: " + feedId, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

}
