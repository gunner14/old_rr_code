/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.commons.exceptions;

import com.xiaonei.commons.exceptions.biz.OpiBizCheckedException;
import com.xiaonei.commons.exceptions.biz.OpiBizException;

/**
 * {@link OpiExceptionUtils}提供了一些静态方法，辅助使用这个package中定义的异常对象
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public class OpiExceptionUtils {

    /**
     * 私有化构造函数，使不能实例化
     */
    private OpiExceptionUtils() {
    }

    /**
     * 判断给定的异常对象是否是规范所定义的<strong>千橡异常</strong>对象，即
     * {@link OpiCheckedException} 或 {@link OpiException}实例或子类实例
     * 
     * @param throwable
     * @return
     */
    public static boolean isOpiException(Throwable throwable) {
        return throwable instanceof OpiException
                || throwable instanceof OpiCheckedException;
    }

    /**
     * 判断给定的异常对象是否是规范所定义的<strong>千橡业务异常</strong>对象，即
     * {@link OpiBizCheckedException} 或 {@link OpiBizException}
     * 实例或子类实例
     * <p>
     * 
     * @param throwable
     * @return
     */
    public static boolean isOpiBizException(Throwable throwable) {
        return throwable instanceof OpiBizException
                || throwable instanceof OpiBizCheckedException;
    }

}
