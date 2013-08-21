package com.renren.sns.wiki.utils.event.impl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.sns.wiki.utils.event.Event;
import com.renren.sns.wiki.utils.event.EventHandler;

/**
 * 事件注册表，用来存储事件对应的handler
 * 
 * <pre>
 * 形式为
 *  Event1 - [Handler1, Handler2, ...]
 *  Event2 - [Handler3, Handler4, ...]
 * </pre>
 * 
 * <p>
 * <strong><font color="red">注意：</font></strong>该结构是非线程安全的，之所以这么做是因为<br>
 * 我们的事件处理器主要在程序启动的时候被注册，那么我们就可以在初始化的时候<br>
 * 在外围加锁处理，而不用我们在这里也对读操作也加锁，做到无锁读取<br>
 * 
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
@SuppressWarnings("rawtypes")
public class EventRegisterTable {

    private Map<Class<? extends Event>, List<EventHandler>> table;

    public EventRegisterTable() {
        table = new HashMap<Class<? extends Event>, List<EventHandler>>();
    }

    /**
     * 为指定类型的事件{@code clazz}注册事件处理器
     * 
     * @param <T> 事件类型
     * @param clazz 事件类
     * @param handler 事件处理器
     */
    public <T extends Event> void registerEvent(Class<T> clazz, EventHandler<T> handler) {
        List<EventHandler> handlers = table.get(clazz);
        if (null == handlers) {
            handlers = new ArrayList<EventHandler>();
            handlers.add(handler);
            table.put(clazz, handlers);
            return ;
        }

        if (!handlers.contains(handler)) {
            handlers.add(handler);
        }
    }

    /**
     * 获取指定事件类型{@code clazz}的处理器（这里由于Java的erasure机制使用了强制转换）
     * 
     * @param <T> 事件类型
     * @param clazz 时间类
     * @return 返回时间处理器列表，如果没有找到返回一个大小0的List
     */
    @SuppressWarnings("unchecked")
    public <T extends Event> List<EventHandler> getHandlers(Class<T> clazz) {
        List<EventHandler> handlers = table.get(clazz);
        if (handlers == null) {
            return Collections.emptyList();
        }

        List<EventHandler> list = new ArrayList<EventHandler>();
        for (EventHandler<? extends Event> item : handlers) {
            EventHandler<T> transfer = (EventHandler<T>) item;
            list.add(transfer);
        }
        return list;
    }
}
