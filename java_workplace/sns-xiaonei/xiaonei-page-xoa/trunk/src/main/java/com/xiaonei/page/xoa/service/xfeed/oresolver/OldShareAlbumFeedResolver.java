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
public class OldShareAlbumFeedResolver extends AbstractVersionedFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_ADMIN_SHARE_ALBUM;
    }

    @Override
    public IContext resolveOldVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("sID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.id", XFeedUtil.getFeedString(feed.get("sID")));
        context.put("share.comment", XFeedUtil.getFeedString(feed.get("comm")));
        context.put("share.album.id", XFeedUtil.getFeedString(feed.get("aID")));
        context.put("share.album.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("share.album.url", XFeedUtil.getFeedString(feed.get("aUrl")));
        context.put("share.album.image", XFeedUtil.getFeedString(feed.get("aSrc")));
        context.put("share.album.count", XFeedUtil.getFeedString(feed.get("count")));
        context.put("share.album.owner.id", XFeedUtil.getFeedString(feed.get("uID")));
        context.put("share.album.owner.name", XFeedUtil.getFeedString(feed.get("uName")));
        return context;
    }

    @Override
    public IContext resolveNewVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("share.id", feed);
        IContext context = getNewShareDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.album.id", XFeedUtil.getFeedString(feed.get("share.resourceId")));
        context.put("share.album.title", XFeedUtil.getFeedString(feed.get("share.title")));
        context.put("share.album.url", XFeedUtil.getFeedString(feed.get("share.url")));
        context.put("share.album.image", XFeedUtil.getFeedString(feed.get("share.thumbFullUrl")));
        context.put("share.album.count", XFeedUtil.getFeedString(feed.get("share.meta.count")));
        context.put("share.album.owner.id", XFeedUtil.getFeedString(feed.get("share.resourceUserId")));
        context.put("share.album.owner.name", XFeedUtil.getFeedString(feed.get("share.resourceUserName")));
        return context;
    }

}
