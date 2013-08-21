package com.renren.sns.wiki.biz.event.handlers;

import org.springframework.beans.factory.InitializingBean;

import com.renren.sns.wiki.utils.event.Event;
import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.EventUtil;

/**
 * @author yi.li@renren-inc.com since 2012-7-5
 *
 * @param <T>
 */
public abstract class AbstractWikiEventHandler<T extends Event> implements InitializingBean,
        EventHandler<T> {

    abstract protected Class<T> getEventClass();

    @Override
    public void afterPropertiesSet() throws Exception {
        // TODO Auto-generated method stub
        EventUtil.getEventManager().subscribeEvent(this, getEventClass());
    }
}
