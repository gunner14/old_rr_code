package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 用在XOA服务的server端，用以确认用户身份
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-23 上午10:37:33
 */
@Interceptor(oncePerRequest = true)
public class UserIdentificationInterceptor extends ControllerInterceptorAdapter {
	
	private HostHolder hostHolder;

    public UserIdentificationInterceptor() {
        setPriority(29900);
    }

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return IgnorePassportValidation.class;
    }
    
    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
    	
    	HttpServletRequest request = inv.getRequest();
        // 之前已经设置过了的访问者
        User host = hostHolder.getUser();

        // 如果之前已经设置过访问者，既然有了，那就算了
        // (这种情况存在于jsp的forward、include或veloctiy的$import.read语句)
        if (host != null) {
            if (logger.isDebugEnabled()) {
                logger.debug("=======host is not null");
            }
            return true;
        }
        String s_userId = request.getHeader("user-id");
        if (s_userId != null) {
        	try {
				int userId = Integer.parseInt(s_userId);
				host = WUserAdapter.getInstance().get(userId);
				if (host != null) {
					if (logger.isDebugEnabled()) {
						logger.debug("host not null:" + userId);
					}
					hostHolder.setUser(host);
				} else {
					if (logger.isDebugEnabled()) {
						logger.debug("host null:" + userId);
					}
				}
			} catch (NumberFormatException e) {
				e.printStackTrace();
			}
        }
        return true;
    }
}
