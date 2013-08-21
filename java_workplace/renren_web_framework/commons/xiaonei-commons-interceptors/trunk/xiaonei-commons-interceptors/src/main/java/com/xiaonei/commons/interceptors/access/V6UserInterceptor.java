package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.vsix.V6UserIdsManager;

/**
 * {@link V6UserInterceptor}
 */
public class V6UserInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private V6UserIdsManager v6UserIdsManager;

    private static final String v6 = "is_v6_user";

    public V6UserInterceptor() {
        this.setPriority(-2000);
    }

    @Override
    public Object before(final Invocation inv) throws Exception {
        try {
            if (this.hostHolder == null) {
                return true;
            }
            final User user = this.hostHolder.getUser();
            if (user == null) {
                return true;
            }

            if (this.v6UserIdsManager == null) {
                return true;
            }
            if (this.v6UserIdsManager.isV6User(user)) {
                inv.getRequest().setAttribute(v6, true);
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return true;
    }


    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return IgnorePassportValidation.class;
    }

}
