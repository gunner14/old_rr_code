package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import com.xiaonei.commons.interceptors.access.annotation.MarkAsFromWap;

public class WapMarkingInterceptor extends ControllerInterceptorAdapter {

    public WapMarkingInterceptor() {
        setPriority(2000);
    }

    @Override
    protected Class<? extends Annotation> getRequiredAnnotationClass() {
        return MarkAsFromWap.class;
    }

    // 以后可以改为override checkAnnoation来实现
    @Override
    protected boolean isForAction(Method actionMethod, Class<?> controllerClazz) {
        MarkAsFromWap markAsFromWap = actionMethod.getAnnotation(MarkAsFromWap.class);
        if (markAsFromWap == null) {
            markAsFromWap = controllerClazz.getAnnotation(MarkAsFromWap.class);
        }
        return markAsFromWap == null ? false : markAsFromWap.value();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        MarkingHelper.forceMarkWapRequest(inv);
        return true;
    }
}
