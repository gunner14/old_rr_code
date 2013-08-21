package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.PassportInParameter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * {@link ParameterPassportValidationInterceptor}
 * 拦截对Web控制器的调用，如果该请求的参数中含有合法的ticket，则把这个ticket对应的用户设置到request的attribute中。
 * 
 * 注：该Interceptor仅用于上传等无法正常使用cookie的业务，在Controller上标注{@link PassportInParameter}可以激活此Interceptor。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-25 下午03:01:50
 */
@Interceptor(oncePerRequest = true)
public class ParameterPassportValidationInterceptor extends ControllerInterceptorAdapter {

	private static final String ticketParamName = "t";
	
    private HostHolder hostHolder;

    public ParameterPassportValidationInterceptor() {
        setPriority(29900);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }
    
    @Override
    protected Class<? extends Annotation> getRequiredAnnotationClass() {
        return PassportInParameter.class;
    }

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return IgnorePassportValidation.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
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

        // 获取HTTP请求中的ticket Cookie，验证其合法性，并从ICE服务中获取对应的用户对象
        Integer guesterId = null;
		String encryptTicket = request.getParameter(ticketParamName);	//从参数中取加密票
		int userId = PassportManager.getInstance().verifyEncryptedTicket(encryptTicket);	//验证加密票，取userId
		if (userId != -1) {	//-1是验证不通过
			guesterId = userId;
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
