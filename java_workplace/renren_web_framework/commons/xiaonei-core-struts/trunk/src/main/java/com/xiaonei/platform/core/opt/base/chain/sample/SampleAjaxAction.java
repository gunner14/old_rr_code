package com.xiaonei.platform.core.opt.base.chain.sample;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BaseAjaxAction;

/**
 * 
 * 一个需要Ajax Action的例子，很简单，只需要继承自BaseAjaxAction并实现一个接口方法
 * 
 * @author Li Weibo
 * @since 2009-2-5
 */
public class SampleAjaxAction extends BaseAjaxAction {

	@Override
	public ActionForward executeAt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		// TODO Auto-generated method stub
		return null;
	}

	

}
