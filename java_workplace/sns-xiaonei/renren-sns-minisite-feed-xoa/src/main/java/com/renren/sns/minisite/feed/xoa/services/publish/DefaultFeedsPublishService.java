package com.renren.sns.minisite.feed.xoa.services.publish;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.minisite.feed.xoa.components.feedqueue.FeedsQueue;
import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;

@Service("feedsPublishService")
public class DefaultFeedsPublishService implements FeedsPublishService {

    @Autowired
    private FeedsQueue feedsQueue;

    protected Log logger = LogFactory.getLog(this.getClass());

    @Override
    public void publish(final int siteid, final long ugcid) {
        if (this.logger.isInfoEnabled()) {
            this.logger.info("DefaultFeedsPublishService.publish\t" + siteid + "\t" + ugcid);
        }
        final BroadcastModel model = new BroadcastModel();
        model.setSiteId(siteid);
        model.setUgcId(ugcid);
        this.feedsQueue.put(model);
    }

}
