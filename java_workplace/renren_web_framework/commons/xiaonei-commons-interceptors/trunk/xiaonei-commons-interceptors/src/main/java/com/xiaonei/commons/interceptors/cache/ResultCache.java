package com.xiaonei.commons.interceptors.cache;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.util.concurrent.TimeUnit;

import com.xiaonei.platform.core.cache.MemCacheKeys;

/**
 * 缓存方法的调用结果,依据Rose框架规范，只缓存Invocation里的Model数据和Instruction对象，
 * 对于Request对象里Attribute里的和页面渲染有关的对象不缓存。注意
 * 
 * @author lookis (comic.liu@gmail.com)
 * 
 */
@Inherited
@Target( { ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface ResultCache {

    /**
     * 时间单位
     * 
     * @return TimeUnit in {@link TimeUnit}
     */
    TimeUnit timeUnit() default TimeUnit.SECONDS;

    /**
     * 默认缓存池
     * 
     * @return String in {@link MemCacheKeys}
     */
    String cachePool() default "default";

    /**
     * 时长
     * 
     * @return time in {@link Long}
     */
    long timeout() default 10l;
}
