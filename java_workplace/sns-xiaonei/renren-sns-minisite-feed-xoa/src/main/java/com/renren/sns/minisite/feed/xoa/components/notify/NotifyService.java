package com.renren.sns.minisite.feed.xoa.components.notify;

import java.util.List;

import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;


public interface NotifyService {

    public void notify(long ugcId, List<Integer> userIds,BroadcastModel model);
}
