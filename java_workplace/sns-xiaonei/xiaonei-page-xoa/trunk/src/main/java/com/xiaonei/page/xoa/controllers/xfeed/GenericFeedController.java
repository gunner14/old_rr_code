package com.xiaonei.page.xoa.controllers.xfeed;

import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.FeedTypeMap;
import com.xiaonei.page.xoa.service.xfeed.XFeedService;

@Path("generic")
public class GenericFeedController {

    @Autowired
    private XFeedService feedService;

    private final static Log logger = LogFactory.getLog("PAGEXOA");

    @Post("publish")
    public Object publish(@Param("user-id") int realUid, @Param("uid") int uid,
            @Param("feedtype") int feedType,
            @JsonParam @Param("feedParam") Map<String, Object> feed,
            @JsonParam @Param("feedReply") PageFeedReply reply) {
        try {
            logger.info("generic : " + feed.toString());
//            if(feedType == FeedDefinition.STATUS_UPDATE){
//                return "@";
//            }
            feedService.publishFeedForGeneric(uid, realUid, feedType, feed, reply);
        } catch (Exception e) {
            logger.error("Publish Feed Error in Controller.", e);
        }
        return "@";
    }

    @Post("remove")
    public Object removeFeed(@Param("user-id") int realUid, @Param("uid") int uid,
            @Param("sourceId") long sourceId, @Param("feedtype") int feedType) {
        try {
            Long pageFeedType = FeedTypeMap.getMappedPageFeed(feedType);
            if (null == pageFeedType) {
                logger.error("Can not find page feed type - in type:" + feedType);
                return "@";
            }
            feedService.removeFeedForGeneric(uid, realUid, feedType, sourceId);
        } catch (Exception e) {
            logger.error("Remove Feed Error in Controller.", e);
        }
        return "@";
    }
}
