package com.renren.sns.minisite.feed.xoa.utils.mq;

import java.util.List;

import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;

/**
 * 新鲜事分队列
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Jul 12, 2011 3:32:04 PM
 * 
 */
public interface BroadcastQueue {

    public boolean isEmpty() ;

    public void put(final BroadcastModel model) throws InterruptedException;

    public BroadcastModel take() throws InterruptedException;

    public List<BroadcastModel> takes(int count);
}
