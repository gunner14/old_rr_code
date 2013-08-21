package com.xiaonei.platform.core.opt.base.chain.sample;

import java.util.ArrayList;
import java.util.List;

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
public class SampleCacheableAction1 extends BasePageAction {

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		
		//your business logic here
		return null;
	}

	@Override
	protected List<String> getDisabledFilterNames() {
		//重写此方法，返回要屏蔽的filter的name的list
		List<String> ret = new ArrayList<String>();
		ret.add(FilterConstants.FILTER_NAME_DisableBrowserCacheFilter);
		return ret;
	}
}
