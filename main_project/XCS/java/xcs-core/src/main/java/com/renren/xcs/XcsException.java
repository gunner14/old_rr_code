package com.renren.xcs;

/**
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public class XcsException extends Exception {

	private static final long serialVersionUID = 6489562398607761406L;

	public XcsException() {
		super();
	}

	public XcsException(String message, Throwable cause) {
		super(message, cause);
	}

	public XcsException(String message) {
		super(message);
	}

	public XcsException(Throwable cause) {
		super(cause);
	}

}
