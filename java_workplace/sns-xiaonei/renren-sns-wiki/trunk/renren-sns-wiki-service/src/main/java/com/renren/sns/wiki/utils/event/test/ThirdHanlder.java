package com.renren.sns.wiki.utils.event.test;

import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.EventUtil;

/**
 * @author yi.li@renren-inc.com since 2012-3-31
 * 
 */
public class ThirdHanlder implements EventHandler<ThirdEvent> {

    @Override
    public void handleEvent(ThirdEvent event) {
        FirstEvent e = new FirstEvent();
        EventUtil.getEventManager().dispatchEvent(e);
        System.out.println("third handler finished");
    }

}
