package com.renren.xoa2.quota.exception;

import java.lang.RuntimeException;

/**
 * Quota 相关的异常类
 * @author Xue Wenxin Aug 16, 2012 11:57:31 AM
 */
public class XoaQuotaException extends RuntimeException {

	private static final long serialVersionUID = 1L;
    
    public XoaQuotaException(String msg) {
        super(msg);
    }
    
    public XoaQuotaException(Throwable cause) {
        super(cause);
    }
    
    public XoaQuotaException(String msg, Throwable cause) {
        super(msg, cause);
    }
}
