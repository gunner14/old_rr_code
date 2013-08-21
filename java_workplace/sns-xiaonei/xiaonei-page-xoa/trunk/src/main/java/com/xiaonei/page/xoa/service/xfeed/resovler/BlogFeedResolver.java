package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageBlogFeed;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

@Component
public class BlogFeedResolver extends AbstractFeedResolver<PageBlogFeed> implements
        FeedResolver<PageBlogFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageBlogFeed.class;
    }

    @Override
    public IContext resolveParam(PageBlogFeed feed, Page page, User curHost, PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_BLOG_PUBLISH, feed.getBlogId());
        context.put("blog.id", XFeedUtil.getFeedString(feed.getBlogId()));
        context.put("blog.type", XFeedUtil.getFeedString(feed.getBlogType()));
        context.put("blog.title", XFeedUtil.getFeedString(feed.getBlogTitle()));
        context.put("blog.digest", XFeedUtil.getFeedString(feed.getBlogDigest()));
        context.put("blog.url", XFeedUtil.getFeedString(feed.getBlogUrl()));
        context.put("blog.video.image", XFeedUtil.getFeedString(feed.getVideoImage()));
        context.put("blog.photo.image", XFeedUtil.getFeedString(feed.getPhotoImage()));
        context.put("blog.photo.orgImage", XFeedUtil.getFeedString(feed.getPhotoOrgImage()));
        context.put("blog.music", XFeedUtil.getFeedString(feed.getMusic()));
        wrapListData("blog.deny.id", feed.getDenyIds(), context);
        wrapListData("blog.allow.id", feed.getAllowIds(), context);
        context.put("blog.tags", XFeedUtil.getFeedString(feed.getBlogTags()));
        context.put("action", XFeedUtil.getFeedString(feed.getAction()));
        return context;
    }

}
