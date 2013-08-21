package com.renren.sns.wiki.utils.event;

import com.renren.sns.wiki.utils.event.impl.EventManagerImpl;
import com.renren.sns.wiki.utils.event.test.FirstEvent;
import com.renren.sns.wiki.utils.event.test.FirstHanlder;
import com.renren.sns.wiki.utils.event.test.SecondEvent;
import com.renren.sns.wiki.utils.event.test.SecondHanlder;
import com.renren.sns.wiki.utils.event.test.TestEvent;
import com.renren.sns.wiki.utils.event.test.TestEventHanlder;
import com.renren.sns.wiki.utils.event.test.ThirdEvent;
import com.renren.sns.wiki.utils.event.test.ThirdHanlder;

/**
 * 和事件有关的工具
 * 
 * <pre>
 * Usage:
 * EventManager manager = EventUtil.getEventManager();
 * // 订阅事件
 * manager.subscribeEvent(new TestEventHanlder(), TestEvent.class);
 * 
 * // 派发事件
 * TestEvent e = new TestEvent("hello, make sure you have recived this!!!");
 * manager.dispatchEvent(e);
 * </pre>
 * 
 * @author yi.li@renren-inc.com since 2012-3-30
 * 
 */
public class EventUtil {

    private static volatile EventManager eventManager;

    private static Object lock = new Object();

    /**
     * 获取EventManager对象（单例）
     */
    public static EventManager getEventManager() {
        if (eventManager == null) {
            synchronized (lock) {
                if (eventManager == null) {
                    eventManager = new EventManagerImpl();
                }
            }
        }

        return eventManager;
    }

    public static void main(String args[]) {
        System.out.println("begin test...");

        EventManager manager = EventUtil.getEventManager();
        manager.subscribeEvent(new TestEventHanlder(), TestEvent.class);
        manager.subscribeEvent(new FirstHanlder(), FirstEvent.class);
        manager.subscribeEvent(new SecondHanlder(), SecondEvent.class);
        manager.subscribeEvent(new ThirdHanlder(), ThirdEvent.class);

        TestEvent e = new TestEvent("hello, make sure you have recived this!!!");
        TestEvent e1 = new TestEvent("hello1, make sure you have recived this!!!");
        TestEvent e2 = new TestEvent("hello2, make sure you have recived this!!!");
        TestEvent e3 = new TestEvent("hello3, make sure you have recived this!!!");
        long start = System.currentTimeMillis();
        manager.dispatchEvent(e);
        manager.dispatchEvent(e1);
        manager.dispatchEvent(e2);
        manager.dispatchEvent(e3);
        long end = System.currentTimeMillis();

        FirstEvent fe = new FirstEvent();
        manager.dispatchEvent(fe);

        System.out.println("finished test..." + (end - start));
    }
}
