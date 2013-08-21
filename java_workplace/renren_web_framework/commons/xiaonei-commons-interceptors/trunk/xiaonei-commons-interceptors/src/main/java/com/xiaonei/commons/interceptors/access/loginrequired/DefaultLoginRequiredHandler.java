package com.xiaonei.commons.interceptors.access.loginrequired;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

import com.xiaonei.commons.interceptors.access.MarkingHelper;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 默认的{@link LoginRequiredHandler}实现
 * 
 * @author Li Weibo
 * @since 2010-1-12 下午02:41:58
 */
public class DefaultLoginRequiredHandler implements LoginRequiredHandler {

	private static final String PARAM_VIEW = "__view"; // 2个下划线
	
    @Override
    public Object deny(Invocation inv) {
        if (MarkingHelper.isFromWapAndMark(inv)) {
            return "r:" + OpiConstants.urlMobile + "/whome.do";
        } else {
        	//如果是一个新版首页的async请求，就返回错误XML片段
        	String view = inv.getParameter(PARAM_VIEW);
        	if (view != null && view.startsWith("async")) {
        		inv.getResponse().setContentType("text/xml");
        		return "@<message><error code=\"1\">未登录</error></message>";
        	}
        	
        	//否则重定向到登录页
            return "r:" + OpiConstants.urlLogin + "/Login.do?rf=r&domain="
                    + OpiConstants.getCurrentDomain() + "&origURL="
                    + getResourceFullLocation(inv.getRequest());
        }
    }

    protected String getResourceFullLocation(HttpServletRequest request) {
        StringBuffer sb = request.getRequestURL();
        String url = sb.toString();
        String queryString = request.getQueryString();
        if (queryString != null) {
            url = url + "?" + queryString;
        }
        try {
            return URLEncoder.encode(url, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            return url;
        }
    }

}
