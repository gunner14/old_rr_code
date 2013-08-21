package com.xiaonei.commons.interceptors.access.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 标注这个类，确认这个所在的控制器或方法的访问不需要做与用户糯米状态验证有关的逻辑
 * 
 * @author weibo.li@opi-corp.com
 * @since 2011-5-27 下午03:37:25
 */
@Target( { ElementType.TYPE, ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
public @interface IgnoreUserStatusImportedValidation {

}
