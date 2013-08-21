package com.xiaonei.platform.core.opt.base.chain.impl.handler;

import org.apache.commons.chain.Context;

import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.BizFilterExceptionHandler;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 处理单个异常的BizFilterExceptionHandler的基类。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午08:51:45
 */
public abstract class ExceptionHandlerBase implements BizFilterExceptionHandler {
	public abstract int getHandledExceptionCode();

	public final void handle(BizFilterException e, Context context) {
		if (e.getCode() != this.getHandledExceptionCode()) {
			throw new IllegalArgumentException(this.getClass().getName() + " can not handler this exception with code: " + e.getCode());
		}
		handleIt(e, (WebContext)context);
	}
	
	public abstract void handleIt (BizFilterException e, WebContext context);
}
