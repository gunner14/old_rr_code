package com.xiaonei.commons.interceptors.access.origurl;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 
 * @author zhiliang.wang@opi-corp.com
 * @author tai.wang@opi-corp.com
 * 
 */
@Inherited
@Target( { ElementType.TYPE, ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface DontPutOrigURL {

}
