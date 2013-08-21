package com.renren.sns.wiki.utils.event.test;

import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.EventUtil;

/**
 * @author yi.li@renren-inc.com since 2012-3-31
 * 
 */
public class SecondHanlder implements EventHandler<SecondEvent> {

    @Override
    public void handleEvent(SecondEvent event) {
        ThirdEvent e = new ThirdEvent();
        EventUtil.getEventManager().dispatchEvent(e);
        System.out.println("second handler finished");
    }

}
