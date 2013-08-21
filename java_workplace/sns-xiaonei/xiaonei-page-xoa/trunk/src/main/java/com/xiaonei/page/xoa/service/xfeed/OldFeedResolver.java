package com.xiaonei.page.xoa.service.xfeed;

import java.util.Map;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-11
 *
 */
public interface OldFeedResolver {

    /**
     * @return
     */
    long getSupportedFeedType();

    /**
     * @param feed
     * @param page
     * @param curHost
     * @param reply
     * @return
     */
    IContext resolveParam(Map<String, Object> feed, Page page, User curHost, PageFeedReply reply);
}
