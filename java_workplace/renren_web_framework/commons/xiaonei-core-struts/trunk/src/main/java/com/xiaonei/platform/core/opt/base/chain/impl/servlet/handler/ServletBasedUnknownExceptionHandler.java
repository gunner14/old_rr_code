package com.xiaonei.platform.core.opt.base.chain.impl.servlet.handler;

import java.io.IOException;

import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.base.chain.impl.handler.ExceptionHandlerBase;

/**
 * 
 * 基于Servlet，处理未知异常。
 * 
 * @author Li Weibo
 * @since 2009-1-4
 */
public class ServletBasedUnknownExceptionHandler extends ExceptionHandlerBase {

	@Override
	public int getHandledExceptionCode() {
		return BizFilterException.CODE_UNKNOWN_EXCEPTION;
	}

	@Override
	public void handleIt(BizFilterException e, WebContext context) {
		HttpServletResponse response = context.getResponse();
		try {
			response.sendRedirect(OpiConstants.urlWww + "/Error.do");
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
