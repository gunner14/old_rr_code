package com.renren.sns.guide.business;

import com.renren.app.share.hot.model.HotShare;
import com.xiaonei.platform.component.tools.StringUtil;
import com.xiaonei.platform.component.xfeed.publish.FeedBuilder;
import com.xiaonei.platform.core.model.User;

public abstract class GuideFeedDispatcher {
	
	/**
     * 截短长标题(评论)
     * 
     * @param title
     * @return
     */
    public String cutLongTitle(String title) {
        if (title != null) {
            title = StringUtil.cutLongEngWord(title, 25);
        }
        return title;
    }
    
    public abstract FeedBuilder getFeedBuilder(User user, HotShare hs, User resourceUser, int client);
    
}
