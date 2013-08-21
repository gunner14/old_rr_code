package com.xiaonei.platform.core.opt.base.chain.impl.handler;

import java.io.IOException;

import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 
 * 处理异常BizFilterException.CODE_BAD_USER_STATUS。
 * 发生此异常的原因是当前用户是被封禁的用户。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午08:51:54
 */
public class BadUserStatusHandler extends ExceptionHandlerBase {

	@Override
	public int getHandledExceptionCode() {
		return BizFilterException.CODE_BAD_USER_STATUS;
	}

	@Override
	public void handleIt(BizFilterException e, WebContext context) {
		HttpServletResponse response = context.getResponse();
		try {
			//次用户已经被封禁了，所以直接让TA退出登录
			response.sendRedirect(OpiConstants.urlLogin + "/Logout.do");
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
