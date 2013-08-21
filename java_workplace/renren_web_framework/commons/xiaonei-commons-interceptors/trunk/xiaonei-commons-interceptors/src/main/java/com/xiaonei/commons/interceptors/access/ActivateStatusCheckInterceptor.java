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
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;

/**
 * {@link ActivateStatusCheckInterceptor}
 * 拦截对Web控制器的调用，判断如果登录的用户状态为需要验证状态，redirect到验证链接
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class ActivateStatusCheckInterceptor extends ControllerInterceptorAdapter {

    private HostHolder hostHolder;

    private String redirect() {
        return OpiConstants.urlMain + "/Newbie.do";
    }

    public ActivateStatusCheckInterceptor() {
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
        if (guester != null) {
            UserConfirmLogic logic = UserConfirmLogic.getInstance();
            if (!logic.iNeedYou(guester) && guester.getStatus() == User.statusActivateVerified) {
                return "r:" + redirect();
            }
        }
        return true;
    }
}
