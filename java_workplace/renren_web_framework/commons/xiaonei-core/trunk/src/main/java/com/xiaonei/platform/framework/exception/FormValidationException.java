package com.xiaonei.platform.framework.exception;


/**
 * 表单验证异常
 * @author fusong.li@opi-corp.com
 * 2008-3-19 下午03:19:00
 */
public class FormValidationException extends XnException {

	public FormValidationException(int code) {
		super(code);
	}

	public FormValidationException(int code, String message) {
		super(code,message);
	}
	public FormValidationException(int code, Throwable cause) {
		super(code,cause);
	}

	public FormValidationException(int code, String message, Throwable cause) {
		super(code,message, cause);
	}

}
