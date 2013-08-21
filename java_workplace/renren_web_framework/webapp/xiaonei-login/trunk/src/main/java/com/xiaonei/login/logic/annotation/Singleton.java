package com.xiaonei.login.logic.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Singleton <br>
 * 需要singleton 的class实现,然后用 {@link com.xiaonei.login.logic.ObjectMaker} 构造
 * 
 * @see com.xiaonei.login.logic.ObjectMaker
 * 
 * @author tai.wang@opi-corp.com Dec 23, 2009 - 3:46:48 PM
 */
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface Singleton {

}
