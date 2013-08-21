package com.renren.xoa.commons.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 标注在Rose controller的入口参数上。相应的{@link JsonParamResolver}
 * 会按照json规则去自动解析这个参数。
 * 
 * 在使用只@{@link JsonParam}来标注参数的时候，参数名会默认使用类简名首字母小写化后的串，
 * 如对于xx.xx.Blog类，框架会用request.getParameter("blog");来获取其参数；
 * 如果需要自定义参数名，可以再使用一个@{@link Param}标注来指定
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-30 下午12:42:20
 */
@Target( { ElementType.PARAMETER })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface JsonParam {
	
}
