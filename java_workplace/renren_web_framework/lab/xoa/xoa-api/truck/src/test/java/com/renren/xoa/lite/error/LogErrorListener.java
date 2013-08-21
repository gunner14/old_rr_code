package com.renren.xoa.lite.error;

import com.renren.xoa.StatusNotOkException;
import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.impl.listener.ServiceFailureListener;

public class LogErrorListener extends ServiceFailureListener<String, RuntimeException>{

	@Override
	public String onError(ServiceFuture<?> future) throws RuntimeException {
		if (future.getCause() instanceof StatusNotOkException) {
			StatusNotOkException e = (StatusNotOkException)future.getCause();
			if (e.getResponse().getStatusCode() == XoaBizErrorBean.STATUS_CODE) {
				XoaBizErrorBean errorBean = e.getResponse().getContentAs(XoaBizErrorBean.class);
				return errorBean.getMessage();
			} else {
				return e.getMessage();
			}
		} else {
			return future.getCause().getMessage();
		}
	}

}
