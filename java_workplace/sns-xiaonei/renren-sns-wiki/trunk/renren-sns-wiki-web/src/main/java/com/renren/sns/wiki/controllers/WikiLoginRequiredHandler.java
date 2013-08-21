package com.renren.sns.wiki.controllers;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.paoding.rose.web.Invocation;

import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.utils.JSONResultUtil;
import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.commons.interceptors.access.loginrequired.DefaultLoginRequiredHandler;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequiredHandler;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 用来处理Ajax请求的Login Required，在原有的默认实现上修改而来，用来适应Ajax请求
 * 
 * @author yi.li@renren-inc.com since 2012-4-11
 * @see DefaultLoginRequiredHandler
 */
public class WikiLoginRequiredHandler implements LoginRequiredHandler {

    private Log logger = LogFactory.getLog(WikiLoginRequiredHandler.class);
    
    private static final String PARAM_VIEW = "__view"; // 2个下划线

    @Override
    public Object deny(Invocation inv) {
        if (logger.isDebugEnabled()) {
            logger.debug("has ajax annotation ? " + inv.getAnnotation(Ajax.class));
        }
        
        if (null == inv.getAnnotation(Ajax.class)) { // 非ajax请求返回跳转页面
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

        int code = WikiExceptionType.LOGIN_REQUIRED.getCode();
        String msg = WikiExceptionType.LOGIN_REQUIRED.getDisplayMsg();
        return "@" + JSONResultUtil.buildJSONResult(code, msg, "");
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
