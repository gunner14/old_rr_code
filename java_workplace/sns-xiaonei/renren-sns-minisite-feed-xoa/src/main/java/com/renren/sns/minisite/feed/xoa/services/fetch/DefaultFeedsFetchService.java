package com.renren.sns.minisite.feed.xoa.services.fetch;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.minisite.feed.xoa.dao.MinisiteFollowerDAO;
import com.renren.sns.minisite.feed.xoa.dao.MinisiteUgcDAO;

@Service("feedsFetchService")
public class DefaultFeedsFetchService implements FeedsFetchService {

    @Autowired
    private MinisiteFollowerDAO followerDAO;

    @Autowired
    private MinisiteUgcDAO ugcDAO;

    protected Log logger = LogFactory.getLog(this.getClass());

    @Override
    public List<Long> getFeeds(final int userId, final int page) {
        if (this.logger.isInfoEnabled()) {
            this.logger.info("DefaultFeedsFetchService.getFeeds.Page\t" + userId + "\t" + page);
        }
        final int start = page * Resource.count;
        long[] ugcids;
        try { //先走cache队列
            //long starttime = System.currentTimeMillis();
            ugcids = this.getFeedIdsFromCache(userId, start);
            /*if (this.logger.isInfoEnabled()) {
                this.logger.info("DefaultFeedsFetchService.getFeeds.SpanFromCache\t" + userId
                        + "\t" + (System.currentTimeMillis() - starttime));
            }*/
        } catch (final Throwable e) { //cache出现问题读一页db
            e.printStackTrace();
            return this.getFeedIdsFromDB(userId, 0);
        }
        if (!ArrayUtils.isEmpty(ugcids) && (ugcids.length > 10)) {
            return Arrays.asList(ArrayUtils.toObject(ugcids));
        }

        if (this.logger.isDebugEnabled()) {
            if (!ArrayUtils.isEmpty(ugcids)) {
                this.logger.debug("DefaultFeedsFetchService.getFeeds():ugcids.len\t"
                        + ugcids.length);
            }
        }
        if (page < 2) { //没有读到cache或cache中小于10，切页号小于2，读DB
            return this.getFeedIdsFromDB(userId, start);
        }
        return Collections.emptyList();

    }

    private long[] getFeedIdsFromCache(final int userId, final int start) {
        return Resource.MINISITE_FEED_CACHE.getCachedUgcIds(userId, start, Resource.count);
    }

    private List<Long> getFeedIdsFromDB(final int userId, final int start) {
        final List<Integer> siteids = this.followerDAO.getSiteIdsFromFollower(userId);
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("DefaultFeedsFetchService.getFeedIdsFromDB.Start\t" + userId + "\t"
                    + start);
        }
        if (CollectionUtils.isEmpty(siteids)) {
            return Collections.emptyList();
        }
        long startTime = System.currentTimeMillis();
        final List<Long> feedUgcIds = this.ugcDAO.getFeedUgcIds(siteids, start, Resource.count);
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("DefaultFeedsFetchService.getFeedIdsFromDB.Span\t"
                    + (System.currentTimeMillis() - startTime));
        }
        return feedUgcIds;
    }

}
