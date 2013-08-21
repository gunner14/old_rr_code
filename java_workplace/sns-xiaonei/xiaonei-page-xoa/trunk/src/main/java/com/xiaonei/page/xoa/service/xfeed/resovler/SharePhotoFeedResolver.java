package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageSharePhotoFeed;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-7
 *
 */
@Component
public class SharePhotoFeedResolver extends AbstractFeedResolver<PageSharePhotoFeed> implements
        FeedResolver<PageSharePhotoFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageSharePhotoFeed.class;
    }

    @Override
    public IContext resolveParam(PageSharePhotoFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_SHARE_PHOTO, feed.getShareId());
        context.put("share.id", XFeedUtil.getFeedString(feed.getShareId()));
        context.put("share.comment", XFeedUtil.getFeedString(feed.getShareComment()));
        context.put("share.album.id", XFeedUtil.getFeedString(feed.getAlbumId()));
        context.put("share.album.title", XFeedUtil.getFeedString(feed.getAlbumTitle()));
        context.put("share.album.url", XFeedUtil.getFeedString(feed.getAlbumUrl()));
        context.put("share.album.owner.id", XFeedUtil.getFeedString(feed.getAlbumOwnerId()));
        context.put("share.album.owner.name", XFeedUtil.getFeedString(feed.getAlbumOwnerName()));
        context.put("share.album.photo.id", XFeedUtil.getFeedString(feed.getPhotoId()));
        context.put("share.album.photo.url", XFeedUtil.getFeedString(feed.getPhotoUrl()));
        context.put("share.album.photo.image", XFeedUtil.getFeedString(feed.getPhotoImage()));
        context.put("share.album.photo.bigImage", XFeedUtil.getFeedString(feed.getPhotoBigImage()));
        context.put("share.album.photo.digest", XFeedUtil.getFeedString(feed.getPhotoDigest()));
        return context;
    }

}
