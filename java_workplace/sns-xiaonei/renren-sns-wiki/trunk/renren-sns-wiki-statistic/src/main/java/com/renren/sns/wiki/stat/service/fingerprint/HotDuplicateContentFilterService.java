package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.service.feed.WikiHotFeedService;
import com.renren.sns.wiki.utils.SpringBeanUtil;

/**
 * 热门新鲜事重复内容过滤器
 * 
 * @author yi.li@renren-inc.com since 2012-7-19
 * 
 */
@Service
public class HotDuplicateContentFilterService {

    private Log logger = LogFactory.getLog(HotDuplicateContentFilterService.class);

    private int PAGE_SIZE = 60;

    @Autowired
    private WikiFeedService wikiFeedService;

    private List<Map<String, Object>> getFeeds(WikiFeedType hotOrLatest, int wikiId, int start,
            int limit) {
        // TODO 系统参数用的太多了，这里要作为重构点
        String without = System.getProperty("noStatus");
        ItemType itemType = null;
        if (!StringUtils.isEmpty(without)) {
            itemType = ItemType.WIHTOUT_STATUS;
        }
        List<Map<String, Object>> feeds = wikiFeedService.getRawFeed(wikiId, null, itemType, start,
                limit, hotOrLatest);

        return feeds;
    }

    private IdentityResult checkDuplicate(int wikiId, Map<String, Object> feed,
            List<IdentityStrategy> strategies) {
        IdentityResult result = new IdentityResult();
        boolean duplicate = false;
        result.setDuplicate(duplicate);

        int feedType = (Integer) feed.get("feedType");

        for (IdentityStrategy strategy : strategies) {
            if (!strategy.supportFeedType(feedType)) {
                continue;
            }

            duplicate = strategy.isDuplicate(wikiId, feedType, feed);
            if (duplicate == true) {
                result.setDuplicate(duplicate);
                result.setStrategyName(strategy.getStrategyName());
                return result;
            }
        }

        return result;
    }

    /**
     * 根据热门内容去过滤
     * 
     * @param wikiId wiki id
     */
    public void doFilter(int wikiId, WikiFeedType hotOrLatest) {

        boolean hasMore = true;
        int i = 0;

        // 这个地方是为了满足并行的需要将它变成了局部变量
        List<IdentityStrategy> strategies = IdentityStrategyFactory.getStrategies();
        List<Long> sweepFeeds = new ArrayList<Long>();

        while (hasMore && i < 20) {

            try {
                // 读内容
                List<Map<String, Object>> feeds = getFeeds(hotOrLatest, wikiId, i * PAGE_SIZE,
                        PAGE_SIZE);
                if (null == feeds || feeds.size() == 0) {
                    hasMore = false;
                    logger.info("Duplicate no more feed wikiId " + wikiId + " page: " + i);
                    break;
                }

                for (Map<String, Object> feed : feeds) {
                    try {
                        IdentityResult result = checkDuplicate(wikiId, feed, strategies);

                        long feedId = Long.parseLong(feed.get("feedId").toString());

                        if (result.isDuplicate()) {
                            logger.warn(String.format(
                                    "Duplicated, sweep wikiId-%d feedId-%d strategy-%s", wikiId,
                                    feedId, result.getStrategyName()));
                            sweepFeeds.add(feedId);
                        }
                    } catch (Exception e) {
                        logger.error("Check Feed isduplicate error: wikiId-" + wikiId + " feed: "
                                + feed, e);
                    }
                }
            } catch (Exception e) {
                logger.error("", e);
            }
            i++;
        }

        // 之所以要放在所有的重复的id都扫除了之后再真正的删除feed是因为，
        // 如果发现了就删除破坏了一开始的分页顺序，会漏掉一部分数据
        WikiHotFeedService hotFeedService = SpringBeanUtil.getInstance().getBean(
                WikiHotFeedService.class);
        for (Long feedId : sweepFeeds) {
            try {
                // TODO something sweep
                if (hotOrLatest == WikiFeedType.HOT) {
                    hotFeedService.deleteHotFeedRecord(wikiId, feedId);
                } else if (hotOrLatest == WikiFeedType.LATEST) {
                    hotFeedService.deleteNewsFeed(wikiId, feedId);
                }
            } catch (Exception e) {
                logger.error("Sweep Feed error wikiId: " + wikiId + " feedId: " + feedId, e);
            }
        }
    }

    public static class IdentityResult {

        private boolean duplicate;

        private String strategyName;

        public boolean isDuplicate() {
            return duplicate;
        }

        public void setDuplicate(boolean duplicate) {
            this.duplicate = duplicate;
        }

        public String getStrategyName() {
            return strategyName;
        }

        public void setStrategyName(String strategyName) {
            this.strategyName = strategyName;
        }
    }
}
