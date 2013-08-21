package com.xiaonei.platform.framework.exception;


/**
 * 权限验证异常
 * @author fusong.li@opi-corp.com
 * 2008-3-19 下午03:19:00
 */
public class PermissionException extends XnException {

	public PermissionException(int code) {
		super(code);
	}

	public PermissionException(int code, String message) {
		super(code,message);
	}
	public PermissionException(int code, Throwable cause) {
		super(code,cause);
	}

	public PermissionException(int code, String message, Throwable cause) {
		super(code,message, cause);
	}

}
