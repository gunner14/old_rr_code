package com.renren.sns.minisite.feed.xoa.utils.mq;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;

public class LocalBroadcastQueue implements BroadcastQueue {

    private final BlockingQueue<BroadcastModel> queue;

    protected Log logger = LogFactory.getLog(this.getClass());

    public LocalBroadcastQueue(final int size) {
        this.queue = new LinkedBlockingQueue<BroadcastModel>(size);
    }

    @Override
    public boolean isEmpty() {
        return this.queue.isEmpty();
    }

    @Override
    public void put(final BroadcastModel model) throws InterruptedException {
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("LocalBroadcastQueue.put():" + model);
        }
        //added by zwang, 2011/08/01
        //System.out.println("==========come in LocalBroadcastQueue::put()");
        this.queue.put(model);
        //System.out.println("======put: siteid:"+model.getSiteId()+", ugcid:"+model.getUgcId());
    }

    @Override
    public BroadcastModel take() throws InterruptedException {
    	//added by zwang, 2011/08/01
    	//System.out.println("==========come in LocalBroadcastQueue::take()========");
    	//System.out.println("=========elements in the queue: "+this.queue.size());
    	
        final BroadcastModel take = this.queue.take();
        if (this.logger.isDebugEnabled()) {
            this.logger.debug("LocalBroadcastQueue.take():" + take);
        }
        return take;
    }

    @Override
    @Deprecated
    public List<BroadcastModel> takes(final int count) {
        final List<BroadcastModel> list = new ArrayList<BroadcastModel>(count);
        for (int i = 0; i < count; i++) {
            list.add(this.queue.remove());
        }
        return list;
    }

    @Override
    public String toString() {
        return this.queue.toString();
    }
}
