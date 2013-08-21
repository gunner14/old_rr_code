package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * {@link PassportValidationInterceptor}
 * 拦截对Web控制器的调用，如果该请求中含有合法的ticket，则把这个ticket对应的用户设置到request的attribute中
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class PassportValidationInterceptor extends ControllerInterceptorAdapter {

    private HostHolder hostHolder;

    public PassportValidationInterceptor() {
        setPriority(29900);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return IgnorePassportValidation.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        if (inv.getParameter("zhiliang.404test") != null) {
            throw new IllegalStateException("404 test by zhiliang.wang 2010.10.26");
        }
        HttpServletRequest request = inv.getRequest();
        // 之前已经设置过了的访问者
        User guester = hostHolder.getUser();

        // 如果之前已经设置过访问者，既然有了，那就算了
        // (这种情况存在于jsp的forward、include或veloctiy的$import.read语句)
        if (guester != null) {
            if (logger.isDebugEnabled()) {
                logger.debug("=======guester is not null");
            }
            return true;
        }
        if (MarkingHelper.isFromWapAndMark(inv)) {
            if (logger.isDebugEnabled()) {
                logger.debug("=======continue; cause: invocation is from wap");
            }
            return true;
        }

        // 获取HTTP请求中的ticket Cookie，验证其合法性，并从ICE服务中获取对应的用户对象
        Integer guesterId = null;
        try {
            guesterId = PassportManager.getInstance().getHostIdFromCookie(request,
                    inv.getResponse());
        } catch (Ice.TimeoutException e) {
            logger.error(e.getMessage(), e);
        }
        if (guesterId != null && guesterId > 0) {
            try {
                guester = WUserAdapter.getInstance().get(guesterId);
            } catch (Ice.TimeoutException e) {
                logger.error(e.getMessage(), e);
            }catch (Ice.UnknownUserException e) {
                logger.error(e.getMessage(), e);
            }
            if (logger.isDebugEnabled()) {
                logger.debug("==========guester=" + guester);
            }
        } else {	//验票不通过的情况从cookie中清掉T票，IM有此需求
        	if (logger.isDebugEnabled()) {
        		logger.debug("passport validation failed, clear ticket from cookie");
        	}
			if (CookieManager.getInstance().getCookie(request,
					PassportManager.loginKey_ticket) != null) {	//有T票的情况下才去清除，没有的情况下不清楚，防止覆盖掉当前的写T票操作
				PassportManager.getInstance().clearTicketFromCookie(inv.getResponse());
			}
        }

        // 最后，将获取的访客用户对象设置到请求属性中
        if (guester != null) {
            if (logger.isDebugEnabled()) {
                logger.debug("==========set guester=" + guester);
            }
            hostHolder.setUser(guester);
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("======guester is  null");
            }
        }
        return true;
    }
}
