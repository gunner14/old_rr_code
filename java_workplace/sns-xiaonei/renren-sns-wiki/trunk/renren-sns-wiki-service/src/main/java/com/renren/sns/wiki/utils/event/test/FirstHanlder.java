package com.renren.sns.wiki.utils.event.test;

import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.EventUtil;

/**
 * @author yi.li@renren-inc.com since 2012-3-31
 * 
 */
public class FirstHanlder implements EventHandler<FirstEvent> {

    @Override
    public void handleEvent(FirstEvent event) {
        SecondEvent e = new SecondEvent();
        EventUtil.getEventManager().dispatchEvent(e);
        System.out.println("first handler finished");
    }

}
