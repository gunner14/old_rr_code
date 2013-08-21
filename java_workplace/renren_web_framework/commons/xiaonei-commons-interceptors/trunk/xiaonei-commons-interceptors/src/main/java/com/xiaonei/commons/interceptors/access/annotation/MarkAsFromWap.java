package com.xiaonei.commons.interceptors.access.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 标注这个类并且把值设置为true，确认这个所在的控制器或方法的访问是来自Wap的
 * 
 * @author zhiliang.wang
 * 
 */
@Target( { ElementType.TYPE, ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
public @interface MarkAsFromWap {

    boolean value() default true;

}
