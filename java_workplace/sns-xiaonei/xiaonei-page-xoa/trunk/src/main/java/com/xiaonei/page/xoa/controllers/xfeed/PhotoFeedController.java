package com.xiaonei.page.xoa.controllers.xfeed;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageUploadPhotoFeed;
import com.xiaonei.page.xoa.service.xfeed.XFeedService;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * @author yi.li@renren-inc.com since 2011-9-28
 * 
 */
@Path("photo")
public class PhotoFeedController {

    private final static Log logger = LogFactory.getLog("PAGEXOA");

    @Autowired
    private XFeedService feedService;

    @Post("publish")
    public Object publish(@Param("user-id") int realUid, @Param("uid") int uid,
            @JsonParam @Param("feedParam") PageUploadPhotoFeed feed,
            @JsonParam @Param("feedReply") PageFeedReply reply) {
        try {
            feedService.publishFeed(uid, realUid, FeedDefUtil.PAGE_PHOTO_PUBLISH,
                    PageUploadPhotoFeed.class, feed, reply);
        } catch (Exception e) {
            logger.error("send blog feed error" , e);
        }
        return "@";
    }
}
