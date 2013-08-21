package com.xiaonei.commons.interceptors.http;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 标注这个类，确认这个所在的控制器或方法的POST访问必须是来自自己的域名
 * 
 * @author zhiliang.wang
 * 
 */
@Target( { ElementType.TYPE, ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
public @interface ResourceProtected {
}
