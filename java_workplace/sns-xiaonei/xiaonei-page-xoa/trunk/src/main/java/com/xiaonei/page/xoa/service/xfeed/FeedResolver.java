package com.xiaonei.page.xoa.service.xfeed;


import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.platform.core.model.User;

/**
 * 用来解析将对应数据解析成为新鲜事数据模板
 * 
 * @author yi.li@renren-inc.com since 2011-9-29
 * 
 */
public interface FeedResolver<T extends PageFeed> {

    /**
     * 获取支持的
     * 
     * @return
     */
    Class<? extends PageFeed> getSupportClass();

    /**
     * 将类型为T的model解析成为Map类型的数据
     * 
     * @param feed
     * @param page
     * @param curHost
     * @param reply
     * @return
     */
    IContext resolveParam(T feed, Page page, User curHost, PageFeedReply reply);
}
