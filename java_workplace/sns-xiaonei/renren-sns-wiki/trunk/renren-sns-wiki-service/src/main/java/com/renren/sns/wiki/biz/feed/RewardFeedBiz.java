package com.renren.sns.wiki.biz.feed;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-20
 * 
 */
@Service
public class RewardFeedBiz {

    private static final Log logger = LogFactory.getLog("errorfeedlog");

    @Autowired
    private FeedOperationBiz feedOperationBiz;

    /**
     * 用于记录有问题的feed,可以根据不同的实现保存到不同的位置
     * 
     * @param wikiId
     * @param feedId
     * @param remark
     */
    public void rewardFeed(int wikiId, String feedId, String remark) {
        rewardFeed(wikiId, Long.parseLong(feedId), remark);
    }

    public void rewardFeed(int wikiId, long feedId, String remark) {
        StringBuilder sb = new StringBuilder();
        sb.append("wikiId=").append(wikiId).append("; feedId=").append(feedId).append("; remark=")
                .append(remark);
        logger.error(sb.toString());
        //异步删除掉 remark=Feed不存在 这种情况的
        if (wikiId != 0) {
            if ("feed is null".equals(remark) || "photo deleted".equals(remark)) {
                asynDeleteFeed(wikiId, feedId);
            }
        }
    }

    /**
     * 异步删除掉feed
     * 
     * @param wikiId
     * @param feedId
     */
    private void asynDeleteFeed(final int wikiId, final long feedId) {
        AsynTaskExecutorUtil.getAsynExecutor().addTask("asynDeleteFeed", feedId + "",
                new Runnable() {
                    @Override
                    public void run() {
                        feedOperationBiz.removeHotFeed(wikiId, feedId);
                    }
                }, 1000);
    }

}
