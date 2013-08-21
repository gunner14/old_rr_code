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
public class OldShareVideoFeedResolver extends AbstractVersionedFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_SHARE_VIDEO;
    }

    @Override
    public IContext resolveOldVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("sID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.id", XFeedUtil.getFeedString(feed.get("sID")));
        context.put("share.comment", XFeedUtil.getFeedString(feed.get("comm")));
        context.put("share.owner.name", XFeedUtil.getFeedString(feed.get("sownerName")));
        context.put("share.owner.url", XFeedUtil.getFeedString(feed.get("sownerUrl")));
        context.put("share.owner.id", XFeedUtil.getFeedString(feed.get("sownerId")));
        context.put("share.video.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("share.video.image", XFeedUtil.getFeedString(feed.get("thumb")));
        context.put("share.video.url", XFeedUtil.getFeedString(feed.get("url")));
        context.put("share.video.scale", XFeedUtil.getFeedString(feed.get("scale")));
        context.put("share.video.fullurl", XFeedUtil.getFeedString(feed.get("fullurl")));
        return context;
    }

    @Override
    public IContext resolveNewVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("share.id", feed);
        IContext context = getNewShareDefaultContext(feed, reply, page, curHost, shareId);
//        context.put("share.owner.name", XFeedUtil.getFeedString(feed.get("share.resourceUserName")));
//        context.put("share.owner.url", XFeedUtil.getFeedString(feed.get("sownerUrl")));   新版中获取不到了，貌似是人人连接才会有这个参数
        context.put("share.owner.id", XFeedUtil.getFeedString(feed.get("share.resourceUserId")));
        context.put("share.video.title", XFeedUtil.getFeedString(feed.get("share.title")));
        context.put("share.video.image", XFeedUtil.getFeedString(feed.get("share.thumbFullUrl")));
        context.put("share.video.url", XFeedUtil.getFeedString(feed.get("share.meta.flashUrl")));
        context.put("share.video.scale", XFeedUtil.getFeedString(feed.get("scale")));   //该参数有肯能是3G用
        context.put("share.video.fullurl", XFeedUtil.getFeedString(feed.get("share.url")));
        return context;
    }

}
