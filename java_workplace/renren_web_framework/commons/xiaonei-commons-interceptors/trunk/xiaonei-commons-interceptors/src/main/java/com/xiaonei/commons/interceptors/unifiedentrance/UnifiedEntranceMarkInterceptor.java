package com.xiaonei.commons.interceptors.unifiedentrance;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 判断是否要对当前用户应用unifiedEntrance策略，并设置标志
 * 
 * @author Li Weibo (weibo.li@opi-corp.com)
 */
@Interceptor(oncePerRequest = true)
public class UnifiedEntranceMarkInterceptor extends ControllerInterceptorAdapter {

	public UnifiedEntranceMarkInterceptor() {
		setPriority(29900 - 1); // 优先级比PassportValidationInterceptor略低，因为要取userId
	}
	
	@Override
	protected Object before(Invocation inv) throws Exception {
		
		if (OpiConstants.isKaixin()) {
    		return true;
    	}
		return true;
	}
}
