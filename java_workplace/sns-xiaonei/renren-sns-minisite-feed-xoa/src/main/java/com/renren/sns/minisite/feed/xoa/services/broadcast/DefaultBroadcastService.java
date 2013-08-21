package com.renren.sns.minisite.feed.xoa.services.broadcast;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.renren.sns.minisite.feed.xoa.components.activeuser.ActiveUserService;
import com.renren.sns.minisite.feed.xoa.components.activeuser.model.UserAct;
import com.renren.sns.minisite.feed.xoa.components.feedqueue.FeedsQueue;
import com.renren.sns.minisite.feed.xoa.components.notify.NotifyService;
import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;

@Service("broadcastService")
public class DefaultBroadcastService implements BroadcastService {

    @Autowired
    @Qualifier("xceCacheActiveUserService")
    private ActiveUserService activeUserService;

    @Autowired
    private FeedsQueue feedsQueue;

    @Autowired
    @Qualifier("hsNotifyService")
    private NotifyService hsNotifyService;

    @Autowired
    @Qualifier("lsNotifyService")
    private NotifyService lsNotifyService;
    

    protected Log logger = LogFactory.getLog(this.getClass());
    @Override
    public void doBroadcast() {
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("DefaultBroadcastService.doBroadcast():");
        }
        final List<BroadcastModel> list = this.feedsQueue.takes(50);
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("DefaultBroadcastService.doBroadcast():" + list);
        }
        for (final BroadcastModel broadcastModel : list) {
            //added by zwang, 2011/07/27
            //System.out.println("============SiteId:"+broadcastModel.getSiteId()+", UgcId:"+broadcastModel.getUgcId());
            final UserAct users = this.activeUserService.getActiveFriends(broadcastModel
                    .getSiteId());
            final List<Integer> active = users.getActiveIds();
            if (this.logger.isDebugEnabled()) {
                this.logger.debug("DefaultBroadcastService.doBroadcast():hs" + active.size());
            }
            
            broadcastModel.setNotifyTime(System.currentTimeMillis());
            
            
            broadcastModel.setActiveCount(active.size());
            this.hsNotifyService.notify(broadcastModel.getUgcId(), active,broadcastModel);
            
            //
            final List<Integer> unactive = users.getUnActiveIds();
            if (this.logger.isDebugEnabled()) {
                this.logger.debug("DefaultBroadcastService.doBroadcast():ls" + unactive.size());
            }
            broadcastModel.setUnActiveCount(unactive.size());
            this.lsNotifyService.notify(broadcastModel.getUgcId(), unactive,broadcastModel);
        }
    }

}
