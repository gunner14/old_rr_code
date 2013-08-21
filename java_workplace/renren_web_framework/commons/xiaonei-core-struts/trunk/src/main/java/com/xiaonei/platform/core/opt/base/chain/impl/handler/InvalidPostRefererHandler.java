package com.xiaonei.platform.core.opt.base.chain.impl.handler;

import java.io.IOException;

import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 处理异常BizFilterException.CODE_INVALID_POST_REFERER。
 * 这类异常是指当前POST请求的Referer是非法的。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午08:51:27
 */
public class InvalidPostRefererHandler extends ExceptionHandlerBase {

	@Override
	public int getHandledExceptionCode() {
		return BizFilterException.CODE_INVALID_POST_REFERER;
	}

	@Override
	public void handleIt(BizFilterException e, WebContext context) {
		HttpServletResponse response = context.getResponse();
		//redirect to home page
		try {
			response.sendRedirect(OpiConstants.urlHome + "/Home.do");
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
