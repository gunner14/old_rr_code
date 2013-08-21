package com.xiaonei.commons.interceptors.access.origurl;

import java.lang.annotation.Annotation;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

@Interceptor(oncePerRequest = true)
public class OrigURLInterceptor extends ControllerInterceptorAdapter {

    public static String REQ_ATTR_ORIG_URL = "origURL";

    public OrigURLInterceptor() {
        setPriority(10000);
    }

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return DontPutOrigURL.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        Invocation head = inv.getHeadInvocation();
        String origURL = (String) head.getRequest().getAttribute(REQ_ATTR_ORIG_URL);
        if (origURL == null) {
            synchronized (head) {
                origURL = (String) head.getRequest().getAttribute(REQ_ATTR_ORIG_URL);
                if (origURL == null) {
                    origURL = new OrigURLImpl(head).value();
                    if (origURL != null) {
                        head.getRequest().setAttribute(REQ_ATTR_ORIG_URL, origURL);
                    }
                }
            }
        }
        return true;
    }
}
