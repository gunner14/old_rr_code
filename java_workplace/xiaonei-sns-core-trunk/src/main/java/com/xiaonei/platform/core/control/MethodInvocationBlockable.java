package com.xiaonei.platform.core.control;

import java.lang.reflect.Method;
import java.util.Collections;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.atomic.AtomicLong;

public class MethodInvocationBlockable extends AbstractLogBlockable {

    final String LOG_FORMAT = "service timeout:%s (%d/%d:%d) %s [%s] @_@";
    final Method method;

    final Object obj;

    final Object[] args;
    /** 超时计数 */
    final static ConcurrentMap<String, AtomicLong> timeoutCounts = new ConcurrentHashMap<String, AtomicLong>();

    public MethodInvocationBlockable(Method method, Object obj, Object[] args, String name) {
        this.method = method;
        this.obj = obj;
        this.args = args;
        this.name=name;
        if(name==null)throw new NullPointerException("'name' must not be null");
        if(timeoutCounts.get(name)==null)//并发？没关系，能丢几次！！！
            timeoutCounts.putIfAbsent(name, new AtomicLong(0));
    }

    @Override
    protected Object execute() throws Exception {
        return method.invoke(obj, args);
    }

    @Override
    protected void logTimeout(long cost,long expect) {
        getLog().error(formatLog(cost,expect));
    }
    
    protected String formatLog(long cost,long expect) {
        AtomicLong cnt = timeoutCounts.get(name);
        return String.format(LOG_FORMAT, //
                this.name//
                ,cost//
                ,expect//
                ,cnt.incrementAndGet()//
                ,method.getName()//
                ,toArguments()//
                );
    }

    protected String toArguments() {
        if(args==null||args.length==0)return "";
        if(args.length==1)return ""+args[0];
        StringBuffer buf = new StringBuffer();
        for(Object o:args)
            buf.append(',').append(o);
        return buf.substring(1);
    }
    
    public static Map<String, AtomicLong> getTimeoutcounts() {
        return Collections.unmodifiableMap(timeoutCounts);
    }
}
