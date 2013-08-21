package com.renren.sns.wiki.utils.remotelog.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import com.renren.sns.wiki.utils.remotelog.LogWhenType;
import com.renren.sns.wiki.utils.remotelog.RemoteLogType;

/**
 * 用来标记controller中的action需要打远程log
 * 
 * <p>
 * 文档地址：<a href="http://sns.d.xiaonei.com/wiki/Wiki.jsp?page=WikiRemoteLog"
 * >http://sns.d.xiaonei.com/wiki/Wiki.jsp?page=WikiRemoteLog</a>
 * 
 * @author yi.li@renren-inc.com since 2012-3-20
 * 
 */
@Target({ ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface RemoteLog {

    /**
     * 远程log的类型（业务类型）
     */
    RemoteLogType type() default RemoteLogType.NONE;

    /**
     * 何时记录远程log
     */
    LogWhenType logWhen() default LogWhenType.ACCESS;

    /**
     * <p>
     * 访问源的参数名
     * <p>
     * 例：如果访问一篇日志，在请求中的参数是"blogId"，那么sourceParam也填做"blogId"
     * 这样在记录log时就能获取到访问源的id
     */
    String sourceParam() default "";

    /**
     * 额外参数。
     * <p>
     * 如果需要记录其他参数，可以在这里将参数名写下，在记录log时会从model中去取这些数据记录下来
     */
    String[] extras() default {};
}
