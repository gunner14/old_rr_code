package com.renren.xoa;

public class XoaException extends Exception {

	private static final long serialVersionUID = 1L;

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
