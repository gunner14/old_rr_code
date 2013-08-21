package com.xiaonei.page.xoa.service.xfeed.oresolver;

import java.util.Map;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.OldFeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;


/**
 * @author yi.li@renren-inc.com since 2011-10-11
 *
 */
@Component
public class OldShareBlogFeedResolver extends AbstractVersionedFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_ADMIN_SHARE_BLOG;
    }

    @Override
    public IContext resolveOldVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("sID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.id", XFeedUtil.getFeedString(feed.get("sID")));
        context.put("share.comment", XFeedUtil.getFeedString(feed.get("comm")));
        context.put("share.blog.owner.id", XFeedUtil.getFeedString(feed.get("uID")));
        context.put("share.blog.owner.name", XFeedUtil.getFeedString(feed.get("uName")));
        context.put("share.blog.owner.certify", XFeedUtil.getFeedString(feed.get("idef")));
        context.put("share.blog.id", XFeedUtil.getFeedString(feed.get("blogID")));
        context.put("share.blog.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("share.blog.digest", XFeedUtil.getFeedString(feed.get("digest")));
        context.put("share.blog.url", XFeedUtil.getFeedString(feed.get("url")));
        return context;
    }

    @Override
    public IContext resolveNewVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("share.id", feed);
        IContext context = getNewShareDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.blog.owner.id", XFeedUtil.getFeedString(feed.get("share.resourceUserId")));
        context.put("share.blog.owner.name", XFeedUtil.getFeedString(feed.get("share.resourceUserName")));
        context.put("share.blog.id", XFeedUtil.getFeedString(feed.get("share.resourceId")));
        context.put("share.blog.title", XFeedUtil.getFeedString(feed.get("share.title")));
        context.put("share.blog.digest", XFeedUtil.getFeedString(feed.get("share.summary")));
        context.put("share.blog.url", XFeedUtil.getFeedString(feed.get("share.url")));
        return context;
    }

}
