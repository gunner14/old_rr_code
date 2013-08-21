package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;
import java.util.List;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.opt.base.PassportManager;

/**
 * 判断当前请求是否来自IM（人人桌面），如果是的话在request对象里设置一个标志，前端可能会用到
 * 
 * @author han.liao@opi-corp.com
 */
public class ImMarkingInterceptor extends ControllerInterceptorAdapter {

    public ImMarkingInterceptor() {
        // 比passport优先级低就好
        setPriority(2000);
    }

    @Override
    protected List<Class<? extends Annotation>> getDenyAnnotationClasses() {
        return createList(2).add(IgnorePassportValidation.class).getList();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        String ticket = PassportManager.getInstance().getCookie(inv.getRequest(),
                PassportManager.loginKey_ticket);
        MarkingHelper.isFromIMAndMark(inv, ticket);
        return true;
    }
}
