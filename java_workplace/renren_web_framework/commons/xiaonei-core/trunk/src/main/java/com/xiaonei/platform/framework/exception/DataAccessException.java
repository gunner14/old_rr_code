package com.xiaonei.platform.framework.exception;


/**
 * 
 * @author lifs
 * 
 */
public class DataAccessException extends XnException {

	public DataAccessException(int code) {
		super(code);
	}

	public DataAccessException(int code, String message) {
		super(code,message);
	}
	public DataAccessException(int code, Throwable cause) {
		super(code,cause);
	}

	public DataAccessException(int code, String message, Throwable cause) {
		super(code,message, cause);
	}

}
