package com.xiaonei.commons.interceptors.unifiedentrance;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginStatus;

/**
 * <p>
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Inherited
@Target( { ElementType.METHOD, ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface CheckUnifiedEntrance {

    String async = "__view=async-html";

    String asyncReload = "__view=async-html-reload";

    boolean value() default true;

    LoginStatus checkWhen() default LoginStatus.ANY;

}
