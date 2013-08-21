package com.xiaonei.page.xoa.service.xfeed;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.platform.component.feed.helper.FeedDefinition;

/**
 * 简单粗暴的方法建立映射
 * 
 * @author yi.li@renren-inc.com since 2011-10-11
 *
 */
public class FeedTypeMap {

    private static final Map<String, Long> feedtypeMap = new HashMap<String, Long>();
    private static final long PHOTO_PUBLISH_MORE = 709;
    
    static {
        feedtypeMap.put(FeedDefinition.STATUS_UPDATE + "", FeedDefUtil.PAGE_STATUS_UPDATE);
        feedtypeMap.put(FeedDefinition.BLOG_PUBLISH + "", FeedDefUtil.PAGE_BLOG_PUBLISH);
        feedtypeMap.put(FeedDefinition.PHOTO_PUBLISH + "", FeedDefUtil.PAGE_SINGLE_PHOTO);
        feedtypeMap.put(PHOTO_PUBLISH_MORE + "", FeedDefUtil.PAGE_PHOTO_PUBLISH);
        feedtypeMap.put(FeedDefinition.SHARE_BLOG + "", FeedDefUtil.PAGE_ADMIN_SHARE_BLOG);
        feedtypeMap.put(FeedDefinition.SHARE_PHOTO + "", FeedDefUtil.PAGE_ADMIN_SHARE_PHOTO);
        feedtypeMap.put(FeedDefinition.SHARE_ALBUM + "", FeedDefUtil.PAGE_ADMIN_SHARE_ALBUM);
        feedtypeMap.put(FeedDefinition.SHARE_LINK + "", FeedDefUtil.PAGE_SHARE_LINK);
        feedtypeMap.put(FeedDefinition.SHARE_MUSIC + "", FeedDefUtil.PAGE_SHARE_MUSIC);
        feedtypeMap.put(FeedDefinition.SHARE_VIDEO + "", FeedDefUtil.PAGE_SHARE_VIDEO);
    }

    public static Long getMappedPageFeed(long feedType) {
        return feedtypeMap.get(feedType + "");
    }
}
