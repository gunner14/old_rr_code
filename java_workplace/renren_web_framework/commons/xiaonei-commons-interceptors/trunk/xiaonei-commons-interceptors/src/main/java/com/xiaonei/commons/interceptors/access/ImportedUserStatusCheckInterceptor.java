package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;
import java.util.List;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * {@link ImportedUserStatusCheckInterceptor}
 * 拦截对Web控制器的调用，判断如果登录的用户状态为尚未转化为人人用户的子站导入用户，则重定向到
 * Guide页面。
 * 
 * @author weibo.li@opi-corp.com
 * @since 2011-5-27 下午03:34:38
 */
@Interceptor(oncePerRequest = true)
public class ImportedUserStatusCheckInterceptor extends ControllerInterceptorAdapter {

    private HostHolder hostHolder;

    private String redirect() {
        return OpiConstants.urlGuide.toString();
    }

    public ImportedUserStatusCheckInterceptor() {
        setPriority(29499);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    protected List<Class<? extends Annotation>> getDenyAnnotationClasses() {
        return createList(1).add(IgnoreUserStatusImportedValidation.class).getList();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        User host = hostHolder.getUser();
        if (host != null) {
            if (host.isImportedUser()) {
            	 return "r:" + redirect();
            }
        }
        return true;
    }
}
