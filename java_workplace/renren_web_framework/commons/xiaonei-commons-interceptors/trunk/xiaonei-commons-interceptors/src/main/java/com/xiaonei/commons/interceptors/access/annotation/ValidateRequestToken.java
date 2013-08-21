package com.xiaonei.commons.interceptors.access.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 在controller的方法上标记这个注释，表示需要进行token的验证。<br/>
 * 
 * @author 王兵[wangchangbing@gmail.com]
 * 
 */
@Target( { ElementType.TYPE, ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
public @interface ValidateRequestToken {
	/**
	 * 该属性可以使post方式下的默认验证取消。
	 * 
	 * @return
	 */
	boolean value() default true;
}
