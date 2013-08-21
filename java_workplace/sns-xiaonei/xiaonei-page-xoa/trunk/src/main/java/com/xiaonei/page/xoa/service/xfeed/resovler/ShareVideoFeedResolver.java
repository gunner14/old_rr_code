package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageShareVideoFeed;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-7
 *
 */
@Component
public class ShareVideoFeedResolver extends AbstractFeedResolver<PageShareVideoFeed> implements
        FeedResolver<PageShareVideoFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageShareVideoFeed.class;
    }

    @Override
    public IContext resolveParam(PageShareVideoFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_SHARE_VIDEO, feed.getShareId());
        context.put("share.id", XFeedUtil.getFeedString(feed.getShareId()));
        context.put("share.comment", XFeedUtil.getFeedString(feed.getShareComment()));
        context.put("share.owner.name", XFeedUtil.getFeedString(feed.getOwnerId()));
        context.put("share.owner.url", XFeedUtil.getFeedString(feed.getOwnerUrl()));
        context.put("share.owner.id", XFeedUtil.getFeedString(feed.getOwnerId()));
        context.put("share.video.title", XFeedUtil.getFeedString(feed.getVideoTitle()));
        context.put("share.video.image", XFeedUtil.getFeedString(feed.getVideoImage()));
        context.put("share.video.url", XFeedUtil.getFeedString(feed.getVideoUrl()));
        context.put("share.video.scale", XFeedUtil.getFeedString(feed.getVideoScale()));
        context.put("share.video.fullurl", XFeedUtil.getFeedString(feed.getVideoFullUrl()));
        return context;
    }

}
