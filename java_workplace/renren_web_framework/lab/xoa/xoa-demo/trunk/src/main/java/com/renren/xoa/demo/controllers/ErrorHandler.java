package com.renren.xoa.demo.controllers;

import net.paoding.rose.web.Invocation;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.commons.interceptors.errorhandler.XoaErrorHandlerBase;

public class ErrorHandler extends XoaErrorHandlerBase{

	@Override
	public XoaBizErrorBean wrapError(Invocation inv, Throwable ex) {
		ex.printStackTrace();
		return new XoaBizErrorBean(ex.getMessage());
	}

}
