package com.renren.sns.wiki.biz;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.delegator.xce.FeedLogicDelegator;
import com.renren.sns.wiki.delegator.xce.FeedRetrieverDelegator;
import com.renren.sns.wiki.model.HighLightSummary;
import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;

/**
 * <p>
 * HighLight即Cover下方右侧小型截图取，为各种类型的UGC缩略图集中展示区
 * <p>
 * 该类中主要包含与这块区域相关的业务
 * 
 * @author yi.li@renren-inc.com since 2012-5-2
 * 
 */
@Service
public class HighLightBiz extends BaseBiz {

    private static final List<String> EMPTY_LIST = Collections.emptyList();

    private static final int MAX_FEED = 300;

    private static final int[] PHOTO_FEED = new int[] { 701, 709 };

    private static final int[] VIDEO_FEED = new int[] { 110 };

    @Autowired
    private FeedLogicDelegator feedLogicDelegator;

    @Autowired
    private FeedRetrieverDelegator feedRetrieverDelegator;

    /**
     * 根据wiki id获取wiki的照片highlight信息
     * 
     * @param wikiId
     * @return {@link HighLightSummary}，不会返回null对象
     */
    public HighLightSummary getPhotoHighLight(final int wikiId) throws Exception {
        HighLightSummary summary = CacheDelegator.get(new QueryAction<HighLightSummary>(
                CacheNameSpace.WIKI, CachePrefix.WIKI_HIGHLIGHT_PHOTO, HighLightSummary.class,
                wikiId) {

            @Override
            public HighLightSummary getFromOther() {
                try {
                    return getHighLightSummary(wikiId, PHOTO_FEED, "f_album_photo_image");
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }

        }, null);

        return summary;
    }

    /**
     * 根据wiki id获取wiki的照片highlight信息
     * 
     * @param wikiId
     * @return {@link HighLightSummary}，不会返回null对象
     */
    public HighLightSummary getVideoHighLight(final int wikiId) throws Exception {
        HighLightSummary summary = CacheDelegator.get(new QueryAction<HighLightSummary>(
                CacheNameSpace.WIKI, CachePrefix.WIKI_HIGHLIGHT_VEDIO, HighLightSummary.class,
                wikiId) {

            @Override
            public HighLightSummary getFromOther() {
                try {
                    return getHighLightSummary(wikiId, VIDEO_FEED, "f_share_video_image");
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }

        }, null);

        return summary;
    }

    /**
     * 获取highlight summary 的信息
     * 
     * @param wikiId
     * @param feedTypes
     * @param thumbnailKey
     *        为缩略图在feed数据中的key，可以在feed.d.xiaonei.com上查找对应新鲜事的的2号wap模板
     * @return
     */
    private HighLightSummary getHighLightSummary(int wikiId, int[] feedTypes, String thumbnailKey)
            throws Exception {
        long[] feedIds = feedLogicDelegator.getFeedIdByTypesFromSocialWikiHotByWikiId(wikiId, 0,
                MAX_FEED, feedTypes);
        if (feedIds == null || feedIds.length == 0) {
            return emptyHighLight();
        }

        long[] queryFeedIds = Arrays.copyOf(feedIds, Math.min(5, feedIds.length));
        FeedSet feedData = feedRetrieverDelegator.getFeedDataById("", wikiId, 2, queryFeedIds);

        List<String> thumbnails = extractThumbnails(feedData, thumbnailKey);

        // TODO 这个地方的feedIds可能由于原内容已经删除导致显示的数量和实际的数据量不一致
        // highLight之后可能有其他的策略，如果没有在读取的时候加入校验策略去清除已经删除了的feedId
        return buildHighLight(thumbnails, feedIds.length);
    }

    private List<String> extractThumbnails(FeedSet feedData, String key) {
        Feed[] feeds = feedData.getData();
        if (feeds == null || feeds.length == 0) {
            return Collections.emptyList();
        }

        List<String> thumbnails = new ArrayList<String>();
        for (Feed feed : feeds) {
            Map<String, List<String>> feedMap = feed.getData();
            if (null == feedMap || null == feedMap.get(key)) {
                logger.warn("null value found key " + key + " feedset " + feedMap);
                continue;
            }
            thumbnails.add(feedMap.get(key).get(0));
        }

        return thumbnails;
    }

    private HighLightSummary emptyHighLight() {
        HighLightSummary summary = new HighLightSummary();
        summary.setCount(0);
        summary.setThumbnails(EMPTY_LIST);
        return summary;
    }

    private HighLightSummary buildHighLight(List<String> thumbnails, int count) {
        HighLightSummary summary = new HighLightSummary();
        summary.setCount(count);
        summary.setThumbnails(thumbnails);
        return summary;
    }

    /**
     * 清空某个Wiki的HightCache
     * 
     * @param wikiId wiki id
     */
    public void cleanHightCache(int wikiId) {
        CacheDelegator.delete(CacheNameSpace.WIKI, CachePrefix.WIKI_HIGHLIGHT_PHOTO,
                HighLightSummary.class, wikiId);
        CacheDelegator.delete(CacheNameSpace.WIKI, CachePrefix.WIKI_HIGHLIGHT_VEDIO,
                HighLightSummary.class, wikiId);
    }
}
