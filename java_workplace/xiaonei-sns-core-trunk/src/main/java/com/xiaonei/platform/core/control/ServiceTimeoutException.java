/**
 * $Id: ServiceTimeoutException.java 17104 2010-02-26 01:31:04Z xingtao.shi@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.platform.core.control;


/** Service timeout Exception
 * @author xylz(xylz@live.cn)
 * @since 2010-1-6
 */
public class ServiceTimeoutException extends RuntimeException {
    /**
	 * 
	 */
	private static final long serialVersionUID = 6888598426726721727L;

	public ServiceTimeoutException() {
    }
    public ServiceTimeoutException(String message) {
        super(message);
    }

    public ServiceTimeoutException(Throwable cause) {
        super(cause);
    }

    public ServiceTimeoutException(String message, Throwable cause) {
        super(message, cause);
    }

}
