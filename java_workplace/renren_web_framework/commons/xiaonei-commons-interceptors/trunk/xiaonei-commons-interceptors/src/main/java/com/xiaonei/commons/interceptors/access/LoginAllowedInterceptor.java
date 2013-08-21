package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;
import java.util.List;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * {@link LoginAllowedInterceptor} 拦截对Web控制器的调用，如果用户的状态不允许他登录，嗯，自动退出
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class LoginAllowedInterceptor extends ControllerInterceptorAdapter {

    private String urlLogout() {
        return OpiConstants.urlLogin + "/Logout.do";
    }

    private HostHolder hostHolder;

    public LoginAllowedInterceptor() {
        setPriority(29500);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    protected List<Class<? extends Annotation>> getDenyAnnotationClasses() {
        return createList(2).add(IgnorePassportValidation.class).add(
                IgnoreUserStatusValidation.class).getList();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        User guester = hostHolder.getUser();
        
        //因为糯米的用户合并进来了，用了10以上20一下的状态码，所以这里MOD 10之后再判断
        if (guester != null && (guester.getStatus() % 10) > User.statusActivateVerified) {
            return "r:" + urlLogout();
        }
        return true;
    }
}
