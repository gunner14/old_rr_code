package com.renren.sns.wiki.utils.event.test;

import com.renren.sns.wiki.utils.event.EventHandler;

/**
 * test handler
 * 
 * @author yi.li@renren-inc.com since 2012-3-30
 * 
 */
public class TestEventHanlder implements EventHandler<TestEvent> {

    @Override
    public void handleEvent(TestEvent event) {
        System.out.println(event.getMsg());
        try {
            Thread.sleep(5000);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
