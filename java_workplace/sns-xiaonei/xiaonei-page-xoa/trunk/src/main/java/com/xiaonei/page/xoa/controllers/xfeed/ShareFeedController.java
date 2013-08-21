package com.xiaonei.page.xoa.controllers.xfeed;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageShareAlbumFeed;
import com.xiaonei.page.xoa.model.feed.PageShareBlogFeed;
import com.xiaonei.page.xoa.model.feed.PageShareLinkFeed;
import com.xiaonei.page.xoa.model.feed.PageShareMusicFeed;
import com.xiaonei.page.xoa.model.feed.PageSharePhotoFeed;
import com.xiaonei.page.xoa.model.feed.PageShareVideoFeed;
import com.xiaonei.page.xoa.service.xfeed.XFeedService;


/**
 * @author yi.li@renren-inc.com since 2011-9-28
 *
 */
@Path("share")
public class ShareFeedController {
    private final static Log logger = LogFactory.getLog("PAGEXOA");
    
    @Autowired
    private XFeedService feedService;
    
    @Post("blog")
    public Object shareBlog(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageShareBlogFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply){
        try {
            feedService.publishFeed(uid, uid, FeedDefUtil.PAGE_SHARE_BLOG,
                    PageShareBlogFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
    
    @Post("album")
    public Object shareAlbum(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageShareAlbumFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply){
        try {
            feedService.publishFeed(uid, uid, FeedDefUtil.PAGE_SHARE_ALBUM,
                    PageShareAlbumFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
    
    @Post("photo")
    public Object sharePhoto(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageSharePhotoFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply){
        try {
            feedService.publishFeed(uid, uid, FeedDefUtil.PAGE_SHARE_PHOTO,
                    PageSharePhotoFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
    
    @Post("link")
    public Object shareLink(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageShareLinkFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply){
        try {
            feedService.publishFeed(uid, uid, FeedDefUtil.PAGE_SHARE_LINK,
                    PageShareLinkFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
    
    @Post("video")
    public Object shareVideo(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageShareVideoFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply){
        try {
            feedService.publishFeed(uid, uid, FeedDefUtil.PAGE_SHARE_VIDEO,
                    PageShareVideoFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
    
    @Post("music")
    public Object shareMusic(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageShareMusicFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply){
        try {
            feedService.publishFeed(uid, uid, FeedDefUtil.PAGE_SHARE_MUSIC,
                    PageShareMusicFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
}
