package com.xiaonei.commons.interceptors.access.loginrequired;

import net.paoding.rose.web.Invocation;

/**
 * 当用户没有登录但要访问一个身分验证的资源(控制器、方法)时，将调用此处理器进行处理。
 * 
 * <p>
 * 请实现特有的 {@link LoginRequiredHandler} 放在rose工程的controllers或子目录下以生效
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public interface LoginRequiredHandler {

    /**
     * 拒绝一次请求
     * 
     * @param inv
     * @return
     */
    public Object deny(Invocation inv);
}
