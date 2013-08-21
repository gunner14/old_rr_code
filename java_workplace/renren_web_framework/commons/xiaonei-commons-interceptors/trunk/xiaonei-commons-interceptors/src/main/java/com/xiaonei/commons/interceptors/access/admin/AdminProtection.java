package com.xiaonei.commons.interceptors.access.admin;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

/**
 * 
 * @author xingtao.shi@opi-corp.com
 * 
 */
@Inherited
@Target( { ElementType.TYPE, ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
@LoginRequired
public @interface AdminProtection {

    String id();
}
