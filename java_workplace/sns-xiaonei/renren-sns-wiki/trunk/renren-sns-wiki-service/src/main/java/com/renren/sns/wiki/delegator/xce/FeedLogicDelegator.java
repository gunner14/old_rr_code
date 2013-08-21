package com.renren.sns.wiki.delegator.xce;

import java.util.Arrays;
import java.util.Random;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.platform.component.xfeed.display.impl.FeedRetriever2;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;
import com.xiaonei.xce.feed.FeedLogicAdapter;

/**
 * 新鲜事接口的代理类
 * 
 * @author yi.li@renren-inc.com since 2012-5-2
 * 
 */
@Service
public class FeedLogicDelegator {

    private Log logger = LogFactory.getLog(FeedLogicDelegator.class);

    private FeedLogicAdapter getFeedAdapter() {
        return FeedLogicAdapter.getInstance();
    }

    /**
     * 按新鲜事类型和wikiId获取wiki 最新feed id
     * 
     * @param wikiId wiki ID
     * @param begin 获取数据的起始位置
     * @param limit 对多获取的数据条数
     * @param types 新鲜事的stype
     * @return 新鲜事id列表
     */
    public long[] getFeedIdByTypesFromSocialWikiLatestByWikiId(int wikiId, int begin, int limit,
            int... types) {
        long[] result = this.getFeedAdapter().getFeedidByTypesFromSocialWikiNewsByWikiid(wikiId,
                begin, limit, types);
        if (logger.isDebugEnabled()) {
            logger.debug(String.format(
                    "Get feed id by types, wikiId-%d types-%s begin-%d limit-%d result-size: %s",
                    wikiId, Arrays.toString(types), begin, limit,
                    result == null ? 0 : Arrays.toString(result)));
        }

        return result;
    }

    /**
     * 按新鲜事类型和wikiId获取wiki 热门feed id
     * 
     * @param wikiId wiki ID
     * @param begin 获取数据的起始位置
     * @param limit 对多获取的数据条数
     * @param types 新鲜事的stype
     * @return 新鲜事id列表
     */
    public long[] getFeedIdByTypesFromSocialWikiHotByWikiId(int wikiId, int begin, int limit,
            int... types) {
        long[] result = this.getFeedAdapter().getFeedidByTypesFromSocialWikiHotByWikiid(wikiId,
                begin, limit, types);
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("Get feed id by types, wikiId-%d types-%s result-size: %s",
                    wikiId, Arrays.toString(types), result == null ? 0 : Arrays.toString(result)));
        }

        return result;
    }

    /**
     * 获取userIds用户在wikiId下的新鲜事id
     * 
     * @param userIds
     * @param wikiId
     * @param begin
     * @param limit
     * @param types
     * @return
     */
    public long[] getFeedidByTypesFromSocialWikiByUserid(int[] userIds, int wikiId, int begin,
            int limit, int... types) {
        long[] result = this.getFeedAdapter().getFeedidByTypesFromSocialWikiByUserid(userIds,
                wikiId, types, begin, limit);
        if (logger.isDebugEnabled()) {
            logger.debug(String
                    .format("Get friend feed id by types,begin=%d, limit=%d wikiId-%d, userIds-%s, types-%s result-size: %s",
                            begin, limit, wikiId, userIds == null ? 0 : Arrays.toString(userIds),
                            Arrays.toString(types), result == null ? 0 : Arrays.toString(result)));
        }
        return result;
    }

    /**
     * 修改内容隐私。
     * <p>
     * 用户修改UGC隐私后需要在新鲜事中删除相应的新鲜事，该部分逻辑封在了这个接口中，由新鲜事完成
     * 
     * @param userId source owner
     * @param sourceId ugc id
     * @param stype 新鲜事小类型
     * @param privacy 隐私
     */
    public void changeWikiFeedPrivacy(int userId, long sourceId, int stype, int privacy) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format(
                    "Change feed privacy userId-%s souceId-%s stype-%s privacy-%s", userId,
                    sourceId, stype, privacy));
        }
        this.getFeedAdapter().changeWikiFeedPrivacy(userId, sourceId, stype, privacy);
    }

    /**
     * 
     * @param currentName
     * @param currentID
     * @param feedIds
     * @return
     * @throws Exception
     */
    public FeedSet getFeedSetByIds(String currentName, int currentID, long[] feedIds)
            throws Exception {
        if (logger.isDebugEnabled()) {
            logger.debug("currentName=" + currentName + ", currentID=" + currentID
                    + ", feedIdCount=" + feedIds.length);
        }
        if (currentName == null) {
            currentName = "wiki";
        }
        if (currentID < 0) {
            currentID = 1000 + new Random().nextInt(30000000);
        }
        FeedSet feedSet = FeedRetriever2.getInstance().getByIds2View(currentName, currentID, 2,
                feedIds);

        return feedSet;
    }

    /**
     * 删除热门新鲜事中的某条feed
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    public void removeFromSocialWikiHotByWikiId(int wikiId, long feedId) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("Remove Hot Feed wikiId=%d feedId=%d", wikiId, feedId));
        }

        this.getFeedAdapter().removeFromSocialWikiHotByWikiid(wikiId, feedId);
    }

    /**
     * 重新载入某个Wiki的最热新鲜事
     * 
     * @param wikiId wiki id
     */
    public void reloadFromSocialWikiHotByWikiId(int wikiId) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("Reload Wiki Hot Feed wikiId=%d", wikiId));
        }

        this.getFeedAdapter().reloadFromSocialWikiHotByWikiid(wikiId);
    }

    /**
     * 删除某个上对应的最新新鲜事
     * 
     * @param wikiId wiki id
     * @param feedId feed id
     */
    public void removeFromSocialWikiNewsByWikiId(int wikiId, long feedId) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("Remove NewsFeed wikiId=%d feedId=%d", wikiId, feedId));
        }

        this.getFeedAdapter().removeFromSocialWikiNewsByWikiid(wikiId, feedId);
    }

    /**
     * 根据UGC源获取feedId
     * 
     * @param sourceId UGC源id
     * @param stype feed 小类型
     * @param owner ownerId
     * @return feedId 
     */
    public long getFeedIdBySource(long sourceId, int stype, int owner) {
        long feedId = FeedLogicAdapter.getInstance().getFeedId(sourceId, stype, owner);
        return feedId;
    }
}
