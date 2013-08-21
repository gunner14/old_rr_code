package com.xiaonei.commons.interceptors.presentense;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;

/**
 * {@link PresentenseCookieInterceptor}
 * 拦截对Web控制器的调用，检查访客的在线状态Cookie，如果已经登录并且没有设置的话，将状态设置为在线
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class PresentenseCookieInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    private HostHolder hostHolder;

    @Override
    public Object before(Invocation inv) throws Exception {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        User guester = hostHolder.getUser();
        if (guester != null) {
            String cookieName = "WebOnLineNotice_" + guester.getId();
            boolean presentenseCookie = false;
            Cookie[] cookies = request.getCookies();
            if (cookies != null) {
                for (Cookie cookie : request.getCookies()) {
                    if (cookie.getName().equals(cookieName)) {
                        presentenseCookie = true;
                        break;
                    }
                }
            }
            if (!presentenseCookie) {
                CookieManager.getInstance().saveCookie(response, cookieName, "1", 5 * 60, "/");
            }
        }
        return true;
    }

}
