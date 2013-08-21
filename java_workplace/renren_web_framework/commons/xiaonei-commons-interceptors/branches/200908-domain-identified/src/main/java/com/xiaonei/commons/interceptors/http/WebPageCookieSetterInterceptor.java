package com.xiaonei.commons.interceptors.http;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;

/**
 * {@link WebPageCookieSetterInterceptor}
 * 拦截对Web控制器的调用，用为WebPager添加一个cookie来冒充session id
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class WebPageCookieSetterInterceptor extends ControllerInterceptorAdapter {

    public WebPageCookieSetterInterceptor() {
        setPriority(29600);
    }

    @Override
    public Object before(Invocation invocation) throws Exception {
        HttpServletRequest request = invocation.getRequest();
        HttpServletResponse response = invocation.getResponse();

        // 为WebPager添加一个cookie来冒充session id
        BizFilterUtil.addCookieForIM(request, response);
        return true;
    }
}
