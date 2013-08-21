package com.xiaonei.commons.interceptors.access.track;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;


import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.interceptors.access.track.decision.Decision;
import com.xiaonei.commons.interceptors.access.track.decision.DecisionResolver;
import com.xiaonei.platform.core.utility.AccessTrackUtil;
import com.xiaonei.xce.log.ActiveTrack;

/**
 * {@link AccessTrackInterceptor} 用于拦截对web控制器的调用，获取房客的用户访问行为，记录到服务器中
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class AccessTrackInterceptor extends ControllerInterceptorAdapter {

    private static Log logger = LogFactory.getLog(AccessTrackInterceptor.class);
    private static final String UNIQ_COOKIE_NAME = "anonymid";
    
    public AccessTrackInterceptor() {
        setPriority(29600);
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        DecisionResolver.getDecision(inv);
        
        HttpServletRequest req = inv.getRequest();
        Cookie[] cookies = req.getCookies();

        Cookie uniqCookie = null; 
        if (cookies != null) {
			for (int i = 0; i < cookies.length; ++i) {
				if (cookies[i].getName().equals(UNIQ_COOKIE_NAME)) {
					uniqCookie = cookies[i];
					break;
				}
			}   
        }

        if (uniqCookie == null) {
          uniqCookie = new Cookie(UNIQ_COOKIE_NAME, ActiveTrack.generateAnonIdent());
          uniqCookie.setDomain(".renren.com");  // 各个子域名均有效
          uniqCookie.setMaxAge(3600 * 24 * 365 * 5);
          uniqCookie.setPath("/");
          
          // 该cookie是否生效？
          inv.getResponse().addCookie(uniqCookie);
        }
        inv.getRequest().setAttribute(UNIQ_COOKIE_NAME, uniqCookie.getValue());
        return super.before(inv);
    }

    @Override
    public void afterCompletion(final Invocation inv, Throwable ex) throws Exception {        
        Decision decision = DecisionResolver.getDecision(inv);
        inv.getRequest().setAttribute("_resourceId", inv.getResourceId());
        String anid = (String)inv.getRequest().getAttribute(UNIQ_COOKIE_NAME);
        if (decision != null) {
            logger.debug("Found Decision,call Ice with decision:"
                    + decision.getDecision().toString());
            AccessTrackUtil.trackActivity(anid, inv.getRequest(), decision.getDecision().toString());
        } else {
            logger.debug("Decision not found,call Ice without decision");
            AccessTrackUtil.trackActivity(anid, inv.getRequest(), inv.getResourceId());
        }
    }

}
