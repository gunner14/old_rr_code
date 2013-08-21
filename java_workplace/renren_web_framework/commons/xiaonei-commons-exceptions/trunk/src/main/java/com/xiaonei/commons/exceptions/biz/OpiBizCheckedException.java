/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.commons.exceptions.biz;

import org.springframework.util.Assert;

import com.xiaonei.commons.exceptions.OpiCheckedException;
/**
 * 
 * {@link OpiBizCheckedException} 代表这是一个千橡程序的受检业务异常。
 * <p>
 * 
 * 受检异常意味着这个异常是可恢复的，调用着知道如何处理这个异常。
 * <p>
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */

public class OpiBizCheckedException extends OpiCheckedException {

    private static final long serialVersionUID = 3351764524884377961L;

    protected String errorCode;

    public OpiBizCheckedException(String errorCode) {
        this(errorCode, "error[" + errorCode + "]");
    }

    public OpiBizCheckedException(String errorCode, Throwable cause) {
        this(errorCode, "error[" + errorCode + "]", cause);
    }

    public OpiBizCheckedException(String errorCode, String msg) {
        this(errorCode, msg, null);
    }

    public OpiBizCheckedException(String errorCode, String msg, Throwable cause) {
        super(msg, cause);
        Assert.hasText(errorCode, "code is required");
        this.errorCode = errorCode;
    }

    public String getErrorCode() {
        return errorCode;
    }
}
