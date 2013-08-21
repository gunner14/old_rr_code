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
 * @author yi.li@renren-inc.com since 2011-10-20
 * 
 */
@Component
public class OldSinglePhotoFeedResolver extends AbstractOldFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_SINGLE_PHOTO;
    }

    @Override
    public IContext resolveParam(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long albumId = getSourceId("album.id", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, albumId);
        context.put("origin.url", XFeedUtil.getFeedString(feed.get("origin.url")));
        context.put("origin.title", XFeedUtil.getFeedString(feed.get("origin.title")));
        context.put("album.id", XFeedUtil.getFeedString(feed.get("album.id")));
        context.put("album.title", XFeedUtil.getFeedString(feed.get("album.name")));
        context.put("album.url", XFeedUtil.getFeedString(feed.get("album.url")));
        context.put("album.count", XFeedUtil.getFeedString(feed.get("album.count")));
        context.put("album.photo.id", XFeedUtil.getFeedString(feed.get("photo.id")));
//        context.put("album.photo.source", XFeedUtil.getFeedString(feed.get("source")));
        context.put("album.photo.bigImage", XFeedUtil.getFeedString(feed.get("photo.lUrl")));
        context.put("album.photo.mainImage", XFeedUtil.getFeedString(feed.get("photo.mUrl")));
        context.put("album.photo.image", XFeedUtil.getFeedString(feed.get("photo.hUrl")));
        context.put("album.photo.digest", XFeedUtil.getFeedString(feed.get("photo.title")));
//        context.put("album.photo.imDigest", XFeedUtil.getFeedString(feed.get("imStatus")));
        context.put("album.photo.url", XFeedUtil.getFeedString(feed.get("photo.url")));
        context.put("lbs.id", XFeedUtil.getFeedString(feed.get("lbsId")));
        context.put("lbs.pid", XFeedUtil.getFeedString(feed.get("pid")));
        context.put("lbs.content", XFeedUtil.getFeedString(feed.get("lbsContent")));
        context.put("lbs.longitude", XFeedUtil.getFeedString(feed.get("longitude")));
        context.put("lbs.latitude", XFeedUtil.getFeedString(feed.get("latitude")));
        return context;
    }

}
