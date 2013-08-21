/**
 * 
 */
package com.renren.xoa.commons.exception;

/**
 * @author Lookis (lookisliu@gmail.com)
 * 
 */
public class XoaException extends RuntimeException {

    private static final long serialVersionUID = -8100054656748896265L;

    public XoaException() {
        super();
    }

    public XoaException(String msg) {
        super(msg);
    }
    
    public XoaException(Throwable cause) {
		super(cause);
	}
    
    public XoaException(String msg, Throwable cause) {
    	super(msg, cause);
    }

}
