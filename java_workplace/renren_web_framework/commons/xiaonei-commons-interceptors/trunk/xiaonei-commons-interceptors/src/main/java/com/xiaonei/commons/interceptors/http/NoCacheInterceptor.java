package com.xiaonei.commons.interceptors.http;

import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

/**
 * 
 * {@link NoCacheInterceptor} 拦截对Web控制器的调用，将response设置为不使用HTTP缓存
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class NoCacheInterceptor extends ControllerInterceptorAdapter {

    @Override
    public Object before(Invocation inv) throws Exception {
        HttpServletResponse response = inv.getResponse();

        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", 0);

        return true;
    }
}
