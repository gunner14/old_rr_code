package com.renren.sns.minisite.feed.xoa.components.feedqueue;

import java.util.List;

import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;


public interface FeedsQueue {

    public boolean put(final BroadcastModel model);

    public List<BroadcastModel> takes(int count);
}
