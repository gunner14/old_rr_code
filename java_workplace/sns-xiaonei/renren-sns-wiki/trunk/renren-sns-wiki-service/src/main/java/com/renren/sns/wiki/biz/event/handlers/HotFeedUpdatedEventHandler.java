package com.renren.sns.wiki.biz.event.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.HighLightBiz;
import com.renren.sns.wiki.biz.event.HotFeedUpdatedEvent;

/**
 * 重载最热新鲜事的eventhandler，该handler会去调用新鲜事接口重新载入最热数据
 * 
 * @author yi.li@renren-inc.com since 2012-7-5
 * 
 */
@Service
public class HotFeedUpdatedEventHandler extends AbstractWikiEventHandler<HotFeedUpdatedEvent> {

    @Autowired
    private HighLightBiz highLightBiz;

    @Override
    protected Class<HotFeedUpdatedEvent> getEventClass() {
        return HotFeedUpdatedEvent.class;
    }

    @Override
    public void handleEvent(HotFeedUpdatedEvent event) {
        // TODO add reload logic when api is available
        
        // clean cache
        highLightBiz.cleanHightCache(event.getWikiId());
    }
}
