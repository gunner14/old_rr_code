package com.xiaonei.platform.core.opt.base.chain;

import org.apache.commons.chain.Context;

/**
 * 处理BizFilterException的类需要实现此接口。
 * 
 * @author Li Weibo
 * @since 2008-12-28 上午11:06:08
 */
public interface BizFilterExceptionHandler {
	
	/**
	 * 处理异常的方法
	 * @param e 异常对象
	 * @param context 上下文对象
	 */
	public void handle(BizFilterException e, Context context);
}
