package com.xiaonei.platform.core.opt.base.chain.impl.handler;

import java.io.IOException;

import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 处理wap环境下的BizFilterException.CODE_NEED_TO_LOGIN。
 * 
 * @author Li Weibo
 * @since 2009-09-23
 */
public class WapNeedToLoginHandler extends ExceptionHandlerBase {

	@Override
	public int getHandledExceptionCode() {
		return BizFilterException.CODE_NEED_TO_LOGIN;
	}

	@Override
	public void handleIt(BizFilterException e, WebContext context) {
		
		HttpServletResponse response = context.getResponse();
		try {
			response.sendRedirect(OpiConstants.urlMobile + "/whome.do");
			return;
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
