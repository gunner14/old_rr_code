package com.xiaonei.platform.core.opt.base.chain.impl.struts.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

/**
 * 
 * Forward到一个带导航栏的页面的Action需要集成自此基类。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午10:09:57
 */
public abstract class BasePageAction extends BaseCompositeAction {

	private static final String CONFIG_FILE_NAME = "base-page-action-config.xml";
	
	public BasePageAction() {
		super(BasePageAction.class.getResourceAsStream(CONFIG_FILE_NAME));
	}
	
	@Override
	public final ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		return executePt(mapping, form, request, response);
	}

	public abstract ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response);

}
