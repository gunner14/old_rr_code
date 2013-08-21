package com.xiaonei.platform.core.opt.base.chain.sample;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.impl.filter.FilterConstants;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;

/**
 * 
 * 一个不需要禁浏览器缓存的Action的例子
 * 
 * @author Li Weibo
 * @since 2009-2-5
 */
public class SampleCacheableAction2 extends BasePageAction {

	public SampleCacheableAction2() {
		super();
		mediator.deleteFilter(FilterConstants.FILTER_NAME_DisableBrowserCacheFilter);
	}
	
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		
		//your business logic here
		return null;
	}
}
