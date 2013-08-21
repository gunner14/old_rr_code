package com.renren.sns.guide.business.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.renren.app.share.IShareConstants;
import com.renren.app.share.hot.model.HotShare;
import com.renren.sns.guide.business.GuideFeedDispatcher;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.component.xfeed.publish.FeedBuilder;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;

public class GuideBlogFeedDispatcher extends GuideFeedDispatcher{

	@Override
	public FeedBuilder getFeedBuilder(User user, HotShare hs,
			User resourceUser, int client) {
		try {           
            long blogId = hs.getResourceId();
            
            FeedBuilder builder = new FeedBuilder.Builder(FeedDefinition.SHARE_BLOG, user.getId(), user.getName(), hs.getId(), blogId
                    ).action(FeedBuilder.ACTION.PUBLISH).merge(blogId).build();
            builder.put("blogID", blogId);
            builder.put("title", cutLongTitle(hs.getTitle()));
            builder.put("uID", resourceUser.getId());
            builder.put("uName", resourceUser.getName());
            builder.put("net", "");
            builder.put("digest", cutLongTitle(hs.getSummary()));
            builder.put("url", hs.getUrl());
            builder.put("sID", hs.getId());// 分享ID            
            if (client == IShareConstants.CLIENT_TYPE_WAP) {
                builder.put("wap", 1);// wap
            }
            else{
                builder.put("wap", 0);
            }            
            try{
                List<Integer> list = new ArrayList<Integer>();
                list.add(user.getId());
                Map<Integer,WUserCache> userCache = WUserCacheAdapter.getInstance().getUserCacheMap(list);
                if(userCache.get(user.getId()).isCharity()){
                    builder.put("isShowIcon","1");
                }
            }
            catch(Exception e){
                GuideLogger.printLog("批量取用户失败:" + e);
            }

            return builder;
        } catch (Exception e) {
        	GuideLogger.printLog("发送新鲜事失败" + e);
        }
        return null;
	}

}
