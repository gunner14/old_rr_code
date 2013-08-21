package com.xiaonei.page.xoa.service.xfeed.oresolver;

import java.util.Map;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.AbstractOldFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.OldFeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-11
 *
 */
@Component
public class OldBlogFeedResolver extends AbstractOldFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_BLOG_PUBLISH;
    }

    @Override
    public IContext resolveParam(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long blogId = getSourceId("blogID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, blogId);
        context.put("origin.type", XFeedUtil.getFeedString(feed.get("source")));
        context.put("origin.url", XFeedUtil.getFeedString(feed.get("link")));
        context.put("origin.title", XFeedUtil.getFeedString(feed.get("text")));
        context.put("blog.id", XFeedUtil.getFeedString(feed.get("blogID")));
        context.put("blog.type", XFeedUtil.getFeedString(feed.get("letter")));
        context.put("blog.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("blog.digest", XFeedUtil.getFeedString(feed.get("brief")));
        context.put("blog.url", XFeedUtil.getFeedString(feed.get("url")));
        context.put("blog.video.image", XFeedUtil.getFeedString(feed.get("vSrc")));
        context.put("blog.photo.image", XFeedUtil.getFeedString(feed.get("src")));
        context.put("blog.photo.orgImage", XFeedUtil.getFeedString(feed.get("orgSrc")));
        context.put("blog.music", XFeedUtil.getFeedString(feed.get("music")));
        wrapListData("deny", "blog.deny.id", null, feed, context);
        wrapListData("allow", "blog.allow.id", null, feed, context);
        context.put("blog.tags", XFeedUtil.getFeedString(feed.get("blog_tags")));
        context.put("action", XFeedUtil.getFeedString(feed.get("action")));
        return context;
    }

}
