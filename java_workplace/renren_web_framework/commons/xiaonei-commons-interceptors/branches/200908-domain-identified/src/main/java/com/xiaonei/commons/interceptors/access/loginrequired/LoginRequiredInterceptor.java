package com.xiaonei.commons.interceptors.access.loginrequired;

import java.lang.annotation.Annotation;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * {@link LoginRequiredInterceptor} 拦截对Web控制器的调用，确认被标注为
 * {@link LoginRequired} 的控制器类或方法只有在登录的情况下才能被访问
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class LoginRequiredInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    private HostHolder hostHolder;

    public LoginRequiredInterceptor() {
        setPriority(LoginRequired.LOGIN_REQUIRED_PRIV);
    }

    @Override
    public Class<? extends Annotation> getAnnotationClass() {
        return LoginRequired.class;
    }

    @Override
    public Object before(Invocation invocation) throws Exception {
        HttpServletRequest request = invocation.getRequest();
        //
        User guester = hostHolder.getUser();
        if (guester != null) {
            return true;
        }
        //
        if (isFromWap(request)) {
            return "r:" + OpiConstants.urlMobile + "/whome.do";
        } else {
            return "r:" + OpiConstants.urlLogin + "/Login.do?rf=r&origURL="
                    + getResourceFullLocation(request);
        }
    }

    protected boolean isFromWap(HttpServletRequest request) {
        String serverName = request.getServerName();
        if ("xiaonei.cn".equalsIgnoreCase(serverName)
                || OpiConstants.domainMobile.toString().equalsIgnoreCase(serverName)) {
            return true;
        }
        String referer = request.getHeader("referer");
        if ("xiaonei.cn".equalsIgnoreCase(referer)
                || OpiConstants.urlMobile.toString().equalsIgnoreCase(referer)) {
            return true;
        }

        return false;
    }

    protected String getResourceFullLocation(HttpServletRequest request) {
        StringBuffer sb = request.getRequestURL();
        if (request.getQueryString() != null) {
            String encodeUrl = request.getQueryString();
            try {
                encodeUrl = URLEncoder.encode(encodeUrl, "UTF-8");
            } catch (Exception e) {}
            sb.append('?').append(encodeUrl);
        }
        return sb.toString();
    }
}
