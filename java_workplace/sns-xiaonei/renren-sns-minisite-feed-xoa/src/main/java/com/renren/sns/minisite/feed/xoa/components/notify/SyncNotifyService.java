package com.renren.sns.minisite.feed.xoa.components.notify;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.renren.sns.minisite.feed.xoa.dao.FeedsIndexDAO;
import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;
import com.renren.sns.minisite.feed.xoa.utils.feedcache.MinisiteFeedCache;
import com.renren.sns.minisite.feed.xoa.utils.feedcache.Stack;

@Component("syncNotifyService")
public class SyncNotifyService implements NotifyService {

    private static final Stack stack = new MinisiteFeedCache();

    @Autowired
    private FeedsIndexDAO feedsIndexDAO;

    protected Log logger = LogFactory.getLog(this.getClass());

    @Override
    public void notify(final long ugcId, final List<Integer> userIds,BroadcastModel model) {
    	//added by zwang, 2011/08/12
    	//System.out.println("=========come in notify========");
        ((MinisiteFeedCache) SyncNotifyService.stack).setFeedsIndexDAO(this.feedsIndexDAO);
        for (final Integer userId : userIds) {
            SyncNotifyService.stack.pushCachedUgcId(userId, ugcId);
            /*if (this.logger.isDebugEnabled()) {
                this.logger.debug("SyncNotifyService.notify():" + userId + "|" + ugcId);
            }  */          
            //added by zwang, 2011/08/12
            //System.out.println("========notify():" + userId + "|" + ugcId);
        }
    }

}
