package com.xiaonei.commons.interceptors.userconfirmed;

import java.lang.annotation.Annotation;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;

/**
 * 如果用户是未认证的，写入一个cookie
 * 
 * @author zhiliang.wang
 * 
 */
@Interceptor(oncePerRequest = true)
public class UserConfirmCookieInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    HostHolder holder;

    @Override
    protected Class<? extends Annotation> getRequiredAnnotationClass() {
        return EnableUserConfirmCookie.class;
    }

    @Override
    public Object after(Invocation inv, Object instruction) throws Exception {
        User user = holder.getUser();
        if (user != null) {
            HttpServletResponse response = inv.getResponse();
            HttpServletRequest request = inv.getRequest();
            boolean confirmed = UserConfirmLogic.getInstance().isConfirmed(user);
            UserConfirmLogic.saveOrClearNoconfirmCookie(request, response, confirmed);
        }
        return instruction;
    }
}
