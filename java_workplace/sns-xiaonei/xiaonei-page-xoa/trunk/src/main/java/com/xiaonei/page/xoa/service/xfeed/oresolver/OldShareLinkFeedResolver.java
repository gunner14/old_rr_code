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

@Component
public class OldShareLinkFeedResolver extends AbstractVersionedFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_SHARE_LINK;
    }

    @Override
    public IContext resolveOldVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("sID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.id", XFeedUtil.getFeedString(feed.get("sID")));
        context.put("share.comment", XFeedUtil.getFeedString(feed.get("comm")));
        context.put("share.sharedlink.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("share.sharedlink.url", XFeedUtil.getFeedString(feed.get("url")));
        context.put("share.sharedlink.fullurl", XFeedUtil.getFeedString(feed.get("fullurl")));
        context.put("share.sharedlink.image", XFeedUtil.getFeedString(feed.get("image")));
        context.put("share.sharedlink.body", XFeedUtil.getFeedString(feed.get("body")));
        context.put("share.sharedlink.owner.id", XFeedUtil.getFeedString(feed.get("uID")));
        context.put("share.sharedlink.owner.name", XFeedUtil.getFeedString(feed.get("uName")));
        context.put("share.sharedlink.ext", XFeedUtil.getFeedString(feed.get("ext")));
        return context;
    }

    @Override
    public IContext resolveNewVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("share.id", feed);
        IContext context = getNewShareDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.sharedlink.title", XFeedUtil.getFeedString(feed.get("share.title")));
        context.put("share.sharedlink.url", XFeedUtil.getFeedString(feed.get("share.url")));
        context.put("share.sharedlink.fullurl", XFeedUtil.getFeedString(feed.get("share.url")));
        context.put("share.sharedlink.image", XFeedUtil.getFeedString(feed.get("share.thumbFullUrl")));
        context.put("share.sharedlink.body", XFeedUtil.getFeedString(feed.get("share.summary")));
        context.put("share.sharedlink.owner.id", XFeedUtil.getFeedString(feed.get("share.resourceUserId")));
        context.put("share.sharedlink.owner.name", XFeedUtil.getFeedString(feed.get("share.resourceUserName")));
        context.put("share.sharedlink.ext", XFeedUtil.getFeedString(feed.get("ext")));
        return context;
    }

}
