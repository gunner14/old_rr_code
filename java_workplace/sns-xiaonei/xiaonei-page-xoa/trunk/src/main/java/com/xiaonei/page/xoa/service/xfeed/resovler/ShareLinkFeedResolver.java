package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageShareLinkFeed;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-7
 * 
 */
@Component
public class ShareLinkFeedResolver extends AbstractFeedResolver<PageShareLinkFeed> implements
        FeedResolver<PageShareLinkFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageShareLinkFeed.class;
    }

    @Override
    public IContext resolveParam(PageShareLinkFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_SHARE_LINK, feed.getShareId());
        context.put("share.id", XFeedUtil.getFeedString(feed.getShareId()));
        context.put("share.comment", XFeedUtil.getFeedString(feed.getShareComment()));
        context.put("share.sharedlink.title", XFeedUtil.getFeedString(feed.getSharedLinkTitle()));
        context.put("share.sharedlink.url", XFeedUtil.getFeedString(feed.getSharedLinkUrl()));
        context.put("share.sharedlink.fullurl",
                XFeedUtil.getFeedString(feed.getSharedLinkFullUrl()));
        context.put("share.sharedlink.image", XFeedUtil.getFeedString(feed.getSharedLinkImage()));
        context.put("share.sharedlink.body", XFeedUtil.getFeedString(feed.getSharedLinkBody()));
        context.put("share.sharedlink.owner.id",
                XFeedUtil.getFeedString(feed.getSharedLinkOwnerId()));
        context.put("share.sharedlink.owner.name",
                XFeedUtil.getFeedString(feed.getSharedLinkOwnerName()));
        context.put("share.sharedlink.ext", XFeedUtil.getFeedString(feed.getSharedLinkExt()));
        return context;
    }

}
