package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageShareAlbumFeed;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

@Component
public class ShareAlbumFeedResolver extends AbstractFeedResolver<PageShareAlbumFeed> implements
        FeedResolver<PageShareAlbumFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageShareAlbumFeed.class;
    }

    @Override
    public IContext resolveParam(PageShareAlbumFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_SHARE_ALBUM, feed.getShareId());
        context.put("share.id", XFeedUtil.getFeedString(feed.getShareId()));
        context.put("share.comment", XFeedUtil.getFeedString(feed.getShareComment()));
        context.put("share.album.id", XFeedUtil.getFeedString(feed.getAlbumId()));
        context.put("share.album.title", XFeedUtil.getFeedString(feed.getAlbumTitle()));
        context.put("share.album.url", XFeedUtil.getFeedString(feed.getAlbumUrl()));
        context.put("share.album.image", XFeedUtil.getFeedString(feed.getAlbumImage()));
        context.put("share.album.count", XFeedUtil.getFeedString(feed.getAlbumCount()));
        context.put("share.album.owner.id", XFeedUtil.getFeedString(feed.getAlbumOwnerId()));
        context.put("share.album.owner.name", XFeedUtil.getFeedString(feed.getAlbumOwnerName()));
        return context;
    }

}
