package com.renren.sns.wiki.controllers.ajax;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

@Path("/feed")
public class WikiFeedController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiFeedService wikiFeedService;

    /**
     * 返回新鲜事
     * 
     * @param inv
     * @param wikiId
     * @param page
     * @param pageSize
     * @param type
     * @return
     */
    @Get("/${wikiId:\\d+}/${wikiFeedType:\\w+}")
    @RemoteLog(type = RemoteLogType.DISPLAY_FEED_AJAX, extras = { "wikiFeedType", "type", "p",
            "frame" })
    public String hotFeed(Invocation inv, @Param("wikiId") int wikiId,
            @Param("wikiFeedType") String strWikiFeedType, @Param("p") int page,
            @Param("frame") int frame, @Param("size") int pageSize, @Param("type") String type) {

        int frameFeedSize = WikiConstant.FRAME_FEED_SIZE;
        if (frame < 0 || frame > frameFeedSize - 1) {
            return writeJson(WikiExceptionType.UNKNOWN_EXCEPTION.getCode(), "frame取0到"
                    + (frameFeedSize - 1) + "之间的值", "");
        }

        Map<String, Object> map = null;

        User host = hostHolder.getUser();

        WikiFeedType wikiFeedType = WikiFeedType.getEnum(strWikiFeedType);
        if (wikiFeedType == null) {
            return writeJson(new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION));
        }

        if (wikiFeedType == WikiFeedType.FRIEND && host == null) {
            return writeJson(new WikiException(WikiExceptionType.LOGIN_REQUIRED));
        }
        //获取feedIds
        ItemType itemType = ItemType.getItemTypeByValue(type);
        int realPage = page * frameFeedSize + frame;

        map = wikiFeedService.getFeedMap(wikiId, host, itemType, realPage, pageSize, wikiFeedType);

        return writeJsonData(map);
    }
}
