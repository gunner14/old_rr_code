package com.renren.sns.wiki.delegator.xce;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.platform.component.xfeed.display.impl.FeedRetriever2;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;

/**
 * 获取新鲜事数据的代理类
 * 
 * @author yi.li@renren-inc.com since 2012-5-2
 * 
 */
@Service
public class FeedRetrieverDelegator {

    private Log logger = LogFactory.getLog(FeedRetrieverDelegator.class);

    /**
     * 获取新鲜事数据
     * 
     * @param currentUserName 当前用户名
     * @param currentUserId 当前用户ID
     * @param view 新鲜事视图模板号
     * @param feedIds 新鲜事ID列表
     * @return {@link FeedSet}
     * @throws Exception
     */
    public FeedSet getFeedDataById(String currentUserName, int currentUserId, int view,
            long... feedIds) throws Exception {
        FeedSet result = FeedRetriever2.getInstance().getByIds2View(currentUserName, currentUserId,
                view, feedIds);
        if (logger.isDebugEnabled()) {
            logger.debug("Get FeedData by Id, result: " + result == null ? "null result" : result
                    .toString());
        }

        return result;
    }
}
