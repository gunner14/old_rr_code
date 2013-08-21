package com.xiaonei.platform.core.opt.base.chain.impl.struts.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;


/**
 * 
 * 可以匿名使用的Action的基类，只完成两个功能：1. 判断POST请求的referer；2. 禁浏览器缓存。
 * 
 * 注册可以使用这个类作为Action基类。
 * 
 * @author 李伟博
 * @since 2009-3-1 下午03:23:46
 */
public abstract class BaseAnonymousAction extends BaseCompositeAction {

	private static final String CONFIG_FILE_NAME = "base-anonymous-action-config.xml";
	
	public BaseAnonymousAction() {
		super(BaseAnonymousAction.class.getResourceAsStream(CONFIG_FILE_NAME));
	}

	@Override
	public final ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		BizFilterUtil.trackActivity(request);
		return executeAt(mapping, form, request, response);
	}
	
	public abstract ActionForward executeAt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response);
}
