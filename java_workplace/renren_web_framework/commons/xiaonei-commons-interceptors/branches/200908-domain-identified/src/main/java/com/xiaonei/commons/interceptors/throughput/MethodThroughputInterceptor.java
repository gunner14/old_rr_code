package com.xiaonei.commons.interceptors.throughput;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

/**
 * 
 * @author 王志亮 qieqie.wang@gmail.com
 * 
 */
public class MethodThroughputInterceptor extends ControllerInterceptorAdapter {

    private Map<Method, AtomicInteger> map = Collections
            .synchronizedMap(new HashMap<Method, AtomicInteger>());

    public MethodThroughputInterceptor() {
        setPriority(Integer.MAX_VALUE);
    }

    @Override
    public Class<? extends Annotation> getAnnotationClass() {
        return MethodThroughput.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        MethodThroughput throughputAnnotation = inv.getMethod().getAnnotation(
                MethodThroughput.class);
        if (throughputAnnotation != null) {
            AtomicInteger count = map.get(inv.getMethod());
            if (count == null) {
                count = new AtomicInteger(0);
                map.put(inv.getMethod(), count);
            }
            if (count.get() >= throughputAnnotation.maxConcurrent()) {
                return throughputAnnotation.instruction();
            } else {
                count.incrementAndGet();
            }
        }
        return true;
    }

    @Override
    public Object after(Invocation inv, Object instruction) throws Exception {
        if (inv.getMethod().isAnnotationPresent(MethodThroughput.class)) {
            AtomicInteger count = map.get(inv.getMethod());
            if (count != null) {
                count.decrementAndGet();
            }
        }
        return instruction;
    }
}
