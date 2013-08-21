package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageShareBlogFeed;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

@Component
public class ShareBlogFeedResolver extends AbstractFeedResolver<PageShareBlogFeed> implements
        FeedResolver<PageShareBlogFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageShareBlogFeed.class;
    }

    @Override
    public IContext resolveParam(PageShareBlogFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_SHARE_BLOG, feed.getShareId());
        context.put("share.id", XFeedUtil.getFeedString(feed.getShareId()));
        context.put("share.comment", XFeedUtil.getFeedString(feed.getShareComment()));
        context.put("share.blog.owner.id", XFeedUtil.getFeedString(feed.getBlogOwnerId()));
        context.put("share.blog.owner.name", XFeedUtil.getFeedString(feed.getBlogOwnerName()));
        context.put("share.blog.owner.certify", XFeedUtil.getFeedString(feed.getBlogOwnerCertify()));
        context.put("share.blog.id", XFeedUtil.getFeedString(feed.getBlogId()));
        context.put("share.blog.title", XFeedUtil.getFeedString(feed.getBlogTitle()));
        context.put("share.blog.digest", XFeedUtil.getFeedString(feed.getBlogDigest()));
        context.put("share.blog.url", XFeedUtil.getFeedString(feed.getBlogUrl()));
        return context;
    }

}
