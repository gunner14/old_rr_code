package com.renren.sns.wiki.utils.event.impl;

import com.renren.sns.wiki.utils.event.Event;

/**
 * 一个对Event的wrapper，主要用途是为了避免循环事件的发生，对Event记录调用路径{@code invocationPath}<br>
 * 该调用路径由事件派发器填充，如果Hanlder中业务也会继续派发事件，派发器就会检查调用路劲判断是否要完成派发。
 * 
 * @author yi.li@renren-inc.com since 2012-3-31
 * 
 */
public class EventWrapper implements Event {

    private static final long serialVersionUID = 4291169986658102264L;

    /**
     * 调用路径
     */
    private String invocationPath;

    /**
     * 事件
     */
    private Event event;

    public EventWrapper(Event event, String invocationPath) {
        this.event = event;
        this.invocationPath = invocationPath;
    }

    public String getInvocationPath() {
        return invocationPath;
    }

    public void setInvocationPath(String invocationPath) {
        this.invocationPath = invocationPath;
    }

    public Event getEvent() {
        return event;
    }

    public void setEvent(Event event) {
        this.event = event;
    }

}
