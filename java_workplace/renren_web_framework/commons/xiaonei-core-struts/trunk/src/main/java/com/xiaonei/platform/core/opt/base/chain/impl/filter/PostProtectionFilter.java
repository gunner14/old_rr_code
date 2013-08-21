package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import java.net.MalformedURLException;
import java.net.URL;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 对POST请求进行referer验证，防止外站提交。
 * 
 * @author Li Weibo
 * @since 2008-12-31
 */
public class PostProtectionFilter extends AbstractBizFilter {

	@Override
	public boolean executeMe(WebContext webContext) throws BizFilterException {
		HttpServletRequest request = webContext.getRequest();

		if ("POST".equalsIgnoreCase(request.getMethod())) {
			String referer = request.getHeader("Referer");
			if (referer != null && referer.trim().length() > 0) {
				URL url = null;
				try {
					referer = referer.replaceAll("#", "");
					url = new URL(referer);
					String hosturl = url.getHost().toLowerCase();
					if (hosturl.endsWith(OpiConstants.domainMain.toString()) || hosturl.endsWith(OpiConstants.domainStatic.toString())
							|| hosturl.endsWith("5q.com")) {
						//passed filtering
					} else {
						throw new BizFilterException(BizFilterException.CODE_INVALID_POST_REFERER);
					}
				} catch (MalformedURLException e) {
					//refer格式有问题,这种情况理论上应该不会发生，但是后来证实在wap平台上确实发生了，所以作忽略处理
					return false;
					//e.printStackTrace();
					//throw new BizFilterException(BizFilterException.CODE_INVALID_POST_REFERER, e);
				}
			} else {
				// 没有referer的情况，也需要处理，暂时还没有处理，
				// 发生了一种情况：学校资料页，先保存到本地，再用这个页提交，虽然没有referer但是可以提交成功，这是个漏洞
			}
		}
		return false;
	}
}
