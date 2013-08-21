/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.commons.exceptions.biz;

import org.springframework.util.Assert;

import com.xiaonei.commons.exceptions.OpiException;

/**
 * 
 * {@link OpiBizException} 代表这是一个千橡程序的非受检业务异常。
 * <p>
 * 
 * 非受检异常意味着这个异常对调用着来说它无法进行恢复，大部分异常应该属于此种。
 * <p>
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public class OpiBizException extends OpiException {

    private static final long serialVersionUID = 3351764524884377961L;

    protected String errorCode;

    public OpiBizException(String errorCode) {
        this(errorCode, "error[" + errorCode + "]");
    }

    public OpiBizException(String errorCode, Throwable cause) {
        this(errorCode, "error[" + errorCode + "]", cause);
    }

    public OpiBizException(String errorCode, String msg) {
        this(errorCode, msg, null);
    }

    public OpiBizException(String errorCode, String msg, Throwable cause) {
        super(msg, cause);
        Assert.hasText(errorCode, "code is required");
        this.errorCode = errorCode;
    }

    public String getErrorCode() {
        return errorCode;
    }
}
