package com.xiaonei.platform.core.opt.base.chain.impl.struts.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

/**
 * 一个处理Ajax请求的Action需要继承自此基类。
 * 由于处理一个Ajax请求时通常不需要读取导航栏信息，所以继承自此Action可以节省资源。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午10:09:57
 */
public abstract class BaseAjaxAction extends BaseCompositeAction {

	private static final String CONFIG_FILE_NAME = "base-ajax-action-config.xml";
	
	public BaseAjaxAction() {
		super(BaseAjaxAction.class.getResourceAsStream(CONFIG_FILE_NAME));
	}
	
	@Override
	public final ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		return executeAt(mapping, form, request, response);
	}

	public abstract ActionForward executeAt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response);

}
