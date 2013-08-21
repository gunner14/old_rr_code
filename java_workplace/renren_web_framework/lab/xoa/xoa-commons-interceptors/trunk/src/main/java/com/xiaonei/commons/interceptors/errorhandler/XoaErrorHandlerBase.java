package com.xiaonei.commons.interceptors.errorhandler;

import net.paoding.rose.web.ControllerErrorHandler;
import net.paoding.rose.web.Invocation;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.commons.interceptors.ojmapping.BizErrorIntruction;

/**
 * XOA中使用的ControllerErrorHandler的基类。
 * 
 * 它的作用是提供模板方法让子类将异常封装成{@link XoaBizErrorBean}，
 * 并通过{@ BizErrorIntruction}来完成渲染。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-2 下午02:08:57
 */
public abstract class XoaErrorHandlerBase implements ControllerErrorHandler {

	private Log logger = LogFactory.getLog(this.getClass());
	
	@Override
	public Object onError(Invocation inv, Throwable ex) throws Throwable {
		XoaBizErrorBean errorBean = wrapError(inv, ex);
		if (errorBean != null) {
			return new BizErrorIntruction(errorBean);
		} else {
			logger.error("Exception unhandled by " + this.getClass().getName(), ex);
		}
		return null;
	}

	/**
	 * 将错误封装为XoaBizErrorBean并返回
	 * 
	 * @param inv 
	 * @param ex
	 * @return
	 */
	public abstract XoaBizErrorBean wrapError(Invocation inv, Throwable ex);	
}
