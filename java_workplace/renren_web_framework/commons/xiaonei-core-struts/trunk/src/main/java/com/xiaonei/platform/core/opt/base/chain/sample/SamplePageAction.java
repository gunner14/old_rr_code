package com.xiaonei.platform.core.opt.base.chain.sample;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;

/**
 * 
 * 一个需要forward到带导航的页面的Action的例子，很简单，只需要继承自BasePageAction并实现一个接口方法
 * 
 * @author Li Weibo
 * @since 2009-2-5
 */
public class SamplePageAction extends BasePageAction {

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		
		//your business logic here
		return null;
	}

}
