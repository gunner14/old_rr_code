package com.xiaonei.platform.core.opt.base.chain.impl.handler;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;

/**
 * 处理BizFilterException.CODE_NEED_TO_LOGIN。
 * 此类异常是指用户尚未登录。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午08:51:19
 */
public class NeedToLoginHandler extends ExceptionHandlerBase {

	@Override
	public int getHandledExceptionCode() {
		return BizFilterException.CODE_NEED_TO_LOGIN;
	}

	@Override
	public void handleIt(BizFilterException e, WebContext context) {
		
		//HttpServletRequest request = (HttpServletRequest) context.get("request");
		//HttpServletResponse response = (HttpServletResponse) context.get("response");
		
		HttpServletRequest request = context.getRequest();
		HttpServletResponse response = context.getResponse();
		
		if (BizFilterUtil.isFromWap(request, response)) {	//从哪来回哪去
			try {
				response.sendRedirect(OpiConstants.urlMobile + "/whome.do");
				return;
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
		
		//跳转到www的login页面
		String uri = request.getRequestURL().toString();
		String redirectURL = OpiConstants.urlLogin + "/Login.do?rf=r";
		
		//传递登录后跳转的URL参数
		if (request.getMethod().equalsIgnoreCase("GET") && !uri.contains("/Login.do")) {	//只有GET请求才传这个参数，过滤掉Login.do防止登录后又跳回登录页
			String origURL = uri;
			String queryString = request.getQueryString();
			if (queryString != null && queryString.length() > 0) {
				origURL += "?" + queryString;
			}
			try {
				redirectURL += "&origURL=" + URLEncoder.encode(origURL, "UTF-8");
			} catch (UnsupportedEncodingException e1) {
				//This will never happen.
			}
		}
		try {
			response.sendRedirect(redirectURL);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
