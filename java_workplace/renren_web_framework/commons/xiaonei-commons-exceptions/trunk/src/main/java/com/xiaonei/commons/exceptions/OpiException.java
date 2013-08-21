/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.commons.exceptions;

import org.springframework.core.NestedRuntimeException;

/**
 * {@link OpiException} 代表这是一个千橡程序的非受检异常。
 * <p>
 * 
 * 非受检异常意味着这个异常对调用着来说它无法进行恢复，大部分异常应该属于此种。
 * <p>
 * 
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */

public class OpiException extends NestedRuntimeException {

    private static final long serialVersionUID = 8632590002096848916L;

    public OpiException(String msg) {
        super(msg);
    }

    public OpiException(String msg, Throwable cause) {
        super(msg, cause);
    }

}
