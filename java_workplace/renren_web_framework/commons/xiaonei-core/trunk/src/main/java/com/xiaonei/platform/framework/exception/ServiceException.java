package com.xiaonei.platform.framework.exception;


/**
 * 
 * @author lifs
 * 
 */
public class ServiceException extends XnException {

	public ServiceException(int code) {
		super(code);
	}

	public ServiceException(int code, String message) {
		super(code,message);
	}
	public ServiceException(int code, Throwable cause) {
		super(code,cause);
	}

	public ServiceException(int code, String message, Throwable cause) {
		super(code,message, cause);
	}

}
