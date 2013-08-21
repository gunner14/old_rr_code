package com.xiaonei.login.logic.validator.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 用于统计登录加密操作所用时间 <br>
 * 
 * @author yong.dou@renren-inc.com
 */
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface EncryptTimer {

}
