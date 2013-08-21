package com.xiaonei.commons.interceptors.access;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;

/**
 * {@link AccessTrackInterceptor} 用于拦截对web控制器的调用，获取房客的用户访问行为，记录到服务器中
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class AccessTrackInterceptor extends ControllerInterceptorAdapter {

	public AccessTrackInterceptor() {
		setPriority(29600);
	}

	@Override
	public Object before(Invocation inv) throws Exception {
		BizFilterUtil.trackActivity(inv.getRequest());
		return true;
	}
}
