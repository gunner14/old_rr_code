package com.renren.xoa.lite.impl.listener;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa.StatusNotOkException;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.renren.xoa.lite.ServiceFuture;

/**
 * 
 * 监听返回，如果返回的是XoaBizErrorBean，则提取之；
 * 如果是其它错误，则log之；
 * 如果是成功的response，则什么也不做。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-1-10 下午12:01:55
 */
public class XoaBizErrorListener extends ServiceFailureListener<XoaBizErrorBean, RuntimeException> {

	protected static final Log default_logger = LogFactory.getLog(XoaBizErrorListener.class);
	
	private final Log myLogger;
	
	public XoaBizErrorListener() {
	    this.myLogger = default_logger;
	}
	
	public XoaBizErrorListener(Log customLogger) {
	    this.myLogger = customLogger;
	}
	
	
	@Override
	public XoaBizErrorBean onError(ServiceFuture<?> future) throws RuntimeException {
		Throwable cause = future.getCause();
		if (cause instanceof StatusNotOkException) {
			StatusNotOkException e = (StatusNotOkException)cause;
			XoaResponse response = e.getResponse();
			int code = response.getStatusCode();
			if (code == XoaBizErrorBean.STATUS_CODE) {
				XoaBizErrorBean error = response.getContentAs(XoaBizErrorBean.class);
				return error;
			} else {
			    myLogger.error("Status code: " + code , cause);
			}
		} else {
		    myLogger.error("", cause);
		}
		return null;
	}
}
