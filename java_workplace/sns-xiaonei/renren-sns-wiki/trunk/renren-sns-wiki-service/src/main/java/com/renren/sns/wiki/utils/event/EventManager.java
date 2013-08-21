package com.renren.sns.wiki.utils.event;

/**
 * 时间管理器
 * <p>
 * 主要完成两个工作：
 * <ul>
 * <li>派发事件</li>
 * <li>订阅事件并注册事件处理器</li>
 * </ul>
 * 
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
public interface EventManager {

    /**
     * 派发事件
     * 
     * @param event
     */
    public <T extends Event> void dispatchEvent(T event);

    /**
     * 订阅指定类型的事件{@code eventClass}并注册处理器{@code handler}
     * 
     * @param <T> 事件类型
     * @param handler 事件处理器
     * @param eventClass 事件Class
     */
    public <T extends Event> void subscribeEvent(EventHandler<T> handler, Class<T> eventClass);
}
