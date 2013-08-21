package com.renren.sns.wiki.utils.event.exception;

/**
 * 循环事件异常，当事件派发出现回路时触发该事件
 * 
 * @author yi.li@renren-inc.com since 2012-3-31
 * 
 */
public class LoopEventException extends RuntimeException {

    private static final long serialVersionUID = 2766185036893044388L;

    private String invocationPath;

    public LoopEventException(String invocationPath) {
        super("Loop Event detected!!! please check your event handler. invocation path: "
                + invocationPath);
        this.invocationPath = invocationPath;
    }

    public String getInvocationPath() {
        return invocationPath;
    }

    public void setInvocationPath(String invocationPath) {
        this.invocationPath = invocationPath;
    }

}
