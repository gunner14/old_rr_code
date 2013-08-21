package com.renren.xoa.commons.exception;

import com.renren.xoa.commons.exception.XoaException;


public class XoaConnectException extends XoaException {

    private static final long serialVersionUID = 1;
	
    public XoaConnectException() {

    }
    
    public XoaConnectException(String msg) {
        super(msg);
    }

    public XoaConnectException(Throwable cause) {
    	super(cause);
    }
    
    public XoaConnectException(String msg, Throwable cause) {
    	super(msg, cause);
    }
}
