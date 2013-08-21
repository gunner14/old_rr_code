package com.xiaonei.commons.interceptors.access.loginrequired;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.commons.interceptors.ojmapping.BizErrorIntruction;

import net.paoding.rose.web.Invocation;

/**
 * XOA使用的{@link LoginRequiredHandler}实现
 * 
 * @author Li Weibo
 * @since 2010-1-12 下午02:41:58
 */
public class XoaLoginRequiredHandler implements LoginRequiredHandler {
	
    @Override
    public Object deny(Invocation inv) {
    	return new BizErrorIntruction(new XoaBizErrorBean("Login required"));
    }
}
