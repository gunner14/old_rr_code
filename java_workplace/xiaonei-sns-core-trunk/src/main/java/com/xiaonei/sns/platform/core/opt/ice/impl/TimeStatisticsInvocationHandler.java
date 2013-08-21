/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.control.TimeCostUtil;

/**
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class TimeStatisticsInvocationHandler implements InvocationHandler {

    private InvocationHandler handler = null;

    private String className = null;

    private static Log logger = LogFactory.getLog(TimeStatisticsInvocationHandler.class);

    public TimeStatisticsInvocationHandler(InvocationHandler handler) {
        this.handler = handler;
    }

    public TimeStatisticsInvocationHandler(InvocationHandler handler, String className) {
        this(handler);
        this.className = className;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {

        Object ret;
        long begin = TimeCostUtil.logBegin(logger);
        try {
            ret = handler.invoke(proxy, method, args);
        } catch (Exception e) {
            TimeCostUtil.logError(logger, e, "Running Time:" + (System.currentTimeMillis() - begin)
                    + " On " + className + ":" + method.getName());
            throw e;
        }
        TimeCostUtil.logEnd(logger, begin, className, method.getName(), -1);
        return ret;
    }
}
