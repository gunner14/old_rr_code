package com.renren.sns.minisite.feed.xoa.components.feedqueue;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Component;

import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;
import com.renren.sns.minisite.feed.xoa.utils.mq.BroadcastQueue;
import com.renren.sns.minisite.feed.xoa.utils.mq.LocalBroadcastQueue;

@Component("feedsQueue")
public class DefaultFeedsQueue implements FeedsQueue {

    private static final BroadcastQueue queue = new LocalBroadcastQueue(Integer.MAX_VALUE);

    protected Log logger = LogFactory.getLog(this.getClass());

    @Override
    public boolean put(final BroadcastModel model) {
        try {
            DefaultFeedsQueue.queue.put(model);
            return true;
        } catch (final InterruptedException e) {
            this.logger.error("DefaultFeedsQueue.queue.put " + model + " interrputed", e);
            return false;
        }
    }

    @Override
    public List<BroadcastModel> takes(final int count) {
        try {
            return Arrays.asList(DefaultFeedsQueue.queue.take());
        } catch (final InterruptedException e) {
            this.logger.error("DefaultFeedsQueue.queue.get interrputed", e);
            return Collections.emptyList();
        }
    }

}
