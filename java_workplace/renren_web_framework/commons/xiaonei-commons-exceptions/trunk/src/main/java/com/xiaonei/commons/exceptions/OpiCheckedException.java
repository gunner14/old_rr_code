/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.commons.exceptions;

import org.springframework.core.NestedCheckedException;

/**
 * {@link OpiCheckedException} 代表这是一个千橡程序的受检异常。
 * <p>
 * 
 * 受检异常意味着这个异常是可恢复的，调用着知道如何处理这个异常。
 * <p>
 * 
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public class OpiCheckedException extends NestedCheckedException {

    private static final long serialVersionUID = -1249448231049788332L;

    public OpiCheckedException(String msg) {
        super(msg);
    }

    public OpiCheckedException(String msg, Throwable cause) {
        super(msg, cause);
    }

}
