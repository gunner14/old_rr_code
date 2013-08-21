package com.renren.sns.wiki.service.admin;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.feed.FeedOperationBiz;
import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.service.BaseService;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.utils.CycleFlowUtil;
import com.renren.sns.wiki.utils.CycleFlowUtil.CycleUnit;
import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;

/**
 * @author yi.li@renren-inc.com since Aug 6, 2012
 * 
 */
@Service
public class AdminFeedEditorService extends BaseService {

    @Autowired
    private WikiFeedService wikiFeedService;

    @Autowired
    private FeedOperationBiz feedOperationBiz;

    /**
     * 根据关键字删除feed
     * 
     * @param wikiId
     * @param keyWords
     */
    public void deleteHotFeedByKeyWords(final int wikiId, final String keyWords) {
        AsynTaskExecutorUtil.getAsynExecutor().addTask("RemoveHotFeedsByKeyWords",
                wikiId + "_" + keyWords, new Runnable() {

                    @Override
                    public void run() {
                        asyncRemoveHotFeeds(wikiId, keyWords);
                    }
                });
    }

    private void asyncRemoveHotFeeds(final int wikiId, final String keyWords) {
        final List<Long> feedIds = new ArrayList<Long>();
        CycleFlowUtil.submitCycleTask(new CycleUnit<Map<String, Object>>() {

            @Override
            public List<Map<String, Object>> queryCycle(int begin, int limit) {
                return wikiFeedService.getRawFeed(wikiId, null, ItemType.BLOG, begin, limit,
                        WikiFeedType.HOT);
            }

            @Override
            public void processCycle(List<Map<String, Object>> datas) {
                for (Map<String, Object> feedData : datas) {
                    String title = (String) feedData.get("title");
                    String summary = (String) feedData.get("summary");
                    Long feedId = Long.parseLong((String) feedData.get("feedId"));
                    if (title.contains(keyWords) || summary.contains(keyWords)) {
                        feedIds.add(feedId);
                    }
                }

            }
        });

        logger.warn(String.format(
                "Remove hot feeds from wiki by key words. wikid: %d keywords: %s feeds: %s",
                wikiId, keyWords, feedIds));
        feedOperationBiz.removeHotFeeds(wikiId, feedIds);
    }
}
