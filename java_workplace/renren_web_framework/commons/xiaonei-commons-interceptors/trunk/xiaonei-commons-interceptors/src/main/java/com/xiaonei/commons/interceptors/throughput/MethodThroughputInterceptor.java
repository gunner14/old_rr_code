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
    public Class<? extends Annotation> getRequiredAnnotationClass() {
        return MethodThroughput.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        MethodThroughput throughputAnnotation = inv.getMethod().getAnnotation(
                MethodThroughput.class);
        AtomicInteger count = map.get(inv.getMethod());
        if (count == null) {
            count = new AtomicInteger(0);
            map.put(inv.getMethod(), count);
        }
        int currentCount = count.get();
        if (currentCount >= throughputAnnotation.maxConcurrent()) {
            logger.debug("MethodThroughput " + inv.getMethod() + " OverMax:"
                    + throughputAnnotation.maxConcurrent() + " current:" + currentCount);
            return throughputAnnotation.instruction();
        } else {
            count.incrementAndGet();
            inv.setAttribute(MethodThroughput.class.getName(), count);
        }
        return true;
    }

    @Override
    public Object after(Invocation inv, Object instruction) throws Exception {
        decrementCountIfNessary(inv);
        return instruction;
    }

    @Override
    public void afterCompletion(Invocation inv, Throwable ex) throws Exception {
        decrementCountIfNessary(inv);
    }

    private void decrementCountIfNessary(Invocation inv) {
        AtomicInteger count = (AtomicInteger) inv.getAttribute(MethodThroughput.class.getName());
        if (count != null) {
            count.decrementAndGet();
            inv.removeAttribute(MethodThroughput.class.getName());
        }
    }
}
