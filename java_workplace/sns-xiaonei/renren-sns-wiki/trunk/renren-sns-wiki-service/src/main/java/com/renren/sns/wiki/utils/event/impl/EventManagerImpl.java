package com.renren.sns.wiki.utils.event.impl;

import com.renren.sns.wiki.utils.event.Event;
import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.EventManager;

/**
 * 事件管理器的默认实现
 * 
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
public class EventManagerImpl implements EventManager {

    private EventRegisterTable registerTable;

    private EventDispatcher dispatcher;

    private Object registerLock = new Object();

    public EventManagerImpl() {
        registerTable = new EventRegisterTable();
        dispatcher = new EventDispatcher(registerTable);
    }

    @Override
    public <T extends Event> void dispatchEvent(T event) {
        dispatcher.dipatcherEvent(event);
    }

    /**
     * 该实现方式，考虑到我们业务特点建议在WEB Application启动时去注册事件，<br>
     * 动态的注册事件可能导致并发的问题因为为了效率读事件注册表是不加锁的
     * 
     * @see com.renren.sns.wiki.utils.event.EventManager#subscribeEvent(com.renren.sns.wiki.utils.event.EventHandler,
     *      java.lang.Class)
     */
    @Override
    public <T extends Event> void subscribeEvent(EventHandler<T> handler, Class<T> eventClass) {
        synchronized (registerLock) {
            registerTable.registerEvent(eventClass, handler);
        }
    }

}
