package sns.renren.chong.annotations;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 *使用这个注解的controller将会被LoginRequiredInterceptor拦截，该拦截器验证用户是否登录
 *
 *@author wenhong.mo@renren-inc.com 
 */

@Inherited
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface LoginRequired {

}
