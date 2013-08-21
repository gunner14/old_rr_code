package com.xiaonei.platform.core.opt.base.chain.impl.struts.handler;

import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.base.chain.impl.handler.ExceptionHandlerBase;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.ActionContext;

/**
 * 
 * 基于Action，处理未知异常。
 * 
 * @author Li Weibo
 * @since 2009-1-4
 */
public class ActionBasedUnknownExceptionHandler extends ExceptionHandlerBase {

	@Override
	public int getHandledExceptionCode() {
		return BizFilterException.CODE_UNKNOWN_EXCEPTION;
	}

	@Override
	public void handleIt(BizFilterException e, WebContext context) {
		ActionMapping mapping = ((ActionContext)context).getActionMapping();
		mapping.findForward(OpiConstants.forwardError);
	}
}
