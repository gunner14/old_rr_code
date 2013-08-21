package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;
import java.util.List;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;

public class PostFormInterceptor extends ControllerInterceptorAdapter {

    protected static Log logger = LogFactory.getLog(PostFormInterceptor.class);

    public PostFormInterceptor() {
        setPriority(2000);
    }

    @Override
    protected List<Class<? extends Annotation>> getDenyAnnotationClasses() {
        return createList(2).add(IgnorePassportValidation.class).getList();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        inv.addModel("postFormSession", generatePostFormSession(inv));
        return true;
    }

    public static String generatePostFormSession(Invocation inv) {
        String ticket = CookieManager.getInstance().getCookie(inv.getRequest(),
                PassportManager.loginKey_ticket);
        if (ticket != null) {
            String postFormSession = Integer.toString(ticket.hashCode());
            if (logger.isDebugEnabled()) {
                logger.debug("postFormSession=" + postFormSession + "; ticket=" + ticket);
            }
            return postFormSession;
        }

        return "";
    }
}
