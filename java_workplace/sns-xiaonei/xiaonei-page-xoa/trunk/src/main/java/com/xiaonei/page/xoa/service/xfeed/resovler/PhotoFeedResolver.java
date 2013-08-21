package com.xiaonei.page.xoa.service.xfeed.resovler;

import java.util.List;

import org.springframework.stereotype.Component;
import org.springframework.util.Assert;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageUploadPhotoFeed;
import com.xiaonei.page.xoa.model.feed.PhotoVO;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-7
 *
 */
@Component
public class PhotoFeedResolver extends AbstractFeedResolver<PageUploadPhotoFeed> implements
        FeedResolver<PageUploadPhotoFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageUploadPhotoFeed.class;
    }

    @Override
    public IContext resolveParam(PageUploadPhotoFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_PHOTO_PUBLISH, feed.getAlbumId());
        context.put("album.id", XFeedUtil.getFeedString(feed.getAlbumId()));
        context.put("album.title", XFeedUtil.getFeedString(feed.getAlbumTitle()));
        context.put("album.digest", XFeedUtil.getFeedString(feed.getAlbumDigest()));
        context.put("album.url", XFeedUtil.getFeedString(feed.getAlbumUrl()));
        context.put("album.count", XFeedUtil.getFeedString(feed.getAlbumCount()));
        wrapPhoto(context, feed.getPhotos());
        return context;
    }

    private void wrapPhoto(IContext context, List<PhotoVO> photos) {
        Assert.notNull(photos);
        int i = 0;
        for (PhotoVO item : photos) {
            context.put("album.photo." + i + ".id", XFeedUtil.getFeedString(item.getPhotoId()));
            context.put("album.photo." + i + ".image",
                    XFeedUtil.getFeedString(item.getPhotoImage()));
            context.put("album.photo." + i + ".bigImage",
                    XFeedUtil.getFeedString(item.getPhotoBigImage()));
            context.put("album.photo." + i + ".url", XFeedUtil.getFeedString(item.getPhotoUrl()));
            context.put("album.photo." + i + ".desc", XFeedUtil.getFeedString(item.getPhotoDesc()));
            context.put("album.photo." + i + ".descfull",
                    XFeedUtil.getFeedString(item.getPhotoDescfull()));
            i++;
        }
    }

}
