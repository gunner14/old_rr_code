package sns.renren.chong.controllers;

import java.lang.annotation.Annotation;

import sns.renren.chong.annotations.LoginRequired;
import sns.renren.chong.beans.UserBean;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

/**
 *拦截器，拦截所有使用LoginRequired注解的controller对应的url集合。 
 *若用户已登陆，返回true;
 *若用户未登陆，重定向到登陆页面
 *
 *@author wenhong.mo@renren-inc.com
 */
public class LoginRequiredInterceptor extends ControllerInterceptorAdapter {
	
	public LoginRequiredInterceptor() {
		super.setPriority(900);
	}
	
	@Override
	protected Class<? extends Annotation> getRequiredAnnotationClass() {
		return LoginRequired.class;
	}
	
	/**
	 *拦截对应的url，通过session判断用户是否登陆。 
	 *已登录，返回true；
	 *未登陆，将该url作为href参数的值附在重定向url后面，使之登陆成功后自动跳转到对应页面
	 */
	@Override
	protected Object before(Invocation inv) throws Exception {
		UserBean loginUser = (UserBean)inv.getRequest().getSession().getAttribute("user");
		if (loginUser == null) {
			String path = inv.getRequest().getRequestURI();
			return "r:/login/showPage?href=" + path; 
		}
		return true;
	}
	
	
	
}
