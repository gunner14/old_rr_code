package com.xiaonei.platform.core.opt.base.chain.sample;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.base.chain.impl.handler.ExceptionHandlerBase;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;

/**
 * 
 * 一个需要自定义ExceptionHandler的例子
 * 
 * @author Li Weibo
 * @since 2009-2-5
 */
public class SampleCustomizedHandlerAction extends BasePageAction {

	public SampleCustomizedHandlerAction() {
		super();
		//自定义一个handler替换掉默认的handler
		mediator.setExceptionHandler(BizFilterException.CODE_NEED_TO_LOGIN, new ExceptionHandlerBase() {
			@Override
			public int getHandledExceptionCode() {
				return BizFilterException.CODE_NEED_TO_LOGIN;
			}

			@Override
			public void handleIt(BizFilterException e, WebContext context) {
				//your handling code
			}
		});
	}
	
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		
		//your business logic here
		return null;
	}
}
