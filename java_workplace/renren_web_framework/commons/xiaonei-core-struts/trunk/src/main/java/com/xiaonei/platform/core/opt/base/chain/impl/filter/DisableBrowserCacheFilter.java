package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 
 * 禁浏览器缓存
 * 
 * @author Li Weibo
 * @since 2008-12-28 下午10:18:05
 */
public class DisableBrowserCacheFilter extends AbstractBizFilter {
	
	@Override
	public boolean executeMe(WebContext webContext) throws BizFilterException {
		//HttpServletRequest request = webContext.getRequest();
		HttpServletResponse response = webContext.getResponse();
		response.setHeader("Cache-Control", "no-cache");
		response.setHeader("Pragma", "no-cache");
		response.setDateHeader("Expires", 0);
		return false;
	}
}
