package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageDoingFeed;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-9-29
 * 
 */
@Component
public class DoingFeedResolver extends AbstractFeedResolver<PageDoingFeed> implements
        FeedResolver<PageDoingFeed> {

    public Class<? extends PageFeed> getSupportClass() {
        return PageDoingFeed.class;
    }

    public IContext resolveParam(PageDoingFeed feed, Page page, User curHost, PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_STATUS_UPDATE, feed.getStatusId());
        context.put("status.id", XFeedUtil.getFeedString(feed.getStatusId()));
        context.put("status.title", XFeedUtil.getFeedString(feed.getStatusTitle()));
        context.put("status.web.title", XFeedUtil.getFeedString(feed.getStatusWebTitle()));
        context.put("status.raw", XFeedUtil.getFeedString(feed.getStatusRaw()));
        context.put("status.time", XFeedUtil.getFeedString(feed.getStatusTime()));
        context.put("status.src", XFeedUtil.getFeedString(feed.getStatusSrc()));
        context.put("status.streamID", XFeedUtil.getFeedString(feed.getStatusStreamID()));
        context.put("status.from.id", XFeedUtil.getFeedString(feed.getStatusFromId()));
        context.put("status.from.type", XFeedUtil.getFeedString(feed.getStatusFromType()));
        context.put("status.from.name", XFeedUtil.getFeedString(feed.getStatusFromName()));
        context.put("status.re.id", XFeedUtil.getFeedString(feed.getStatusReId()));
        context.put("status.re.title", XFeedUtil.getFeedString(feed.getStatusReTitle()));
        context.put("status.re.raw", XFeedUtil.getFeedString(feed.getStatusReRaw()));
        context.put("status.re.src", XFeedUtil.getFeedString(feed.getStatusReSrc()));
        context.put("lbs.id", XFeedUtil.getFeedString(feed.getLbsId()));
        context.put("lbs.pid", XFeedUtil.getFeedString(feed.getLbsPid()));
        context.put("lbs.content", XFeedUtil.getFeedString(feed.getLbsContent()));
        context.put("lbs.longitude", XFeedUtil.getFeedString(feed.getLbsLongitude()));
        context.put("lbs.latitude", XFeedUtil.getFeedString(feed.getLbsLatitude()));
        return context;
    }

}
