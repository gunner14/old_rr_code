package com.xiaonei.commons.interceptors.access;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

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
    public Object before(Invocation inv) throws Exception {
        User guester = hostHolder.getUser();
        if (guester != null && guester.getStatus() >= User.statusActivateVerify) {
            return "r:" + urlLogout();
        }
        return true;
    }
}
