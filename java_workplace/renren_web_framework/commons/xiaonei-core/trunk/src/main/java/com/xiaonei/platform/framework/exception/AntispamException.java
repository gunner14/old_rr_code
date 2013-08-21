package com.xiaonei.platform.framework.exception;


/**
 * anti spam 异常
 * @author fusong.li@opi-corp.com
 * 2008-3-19 下午03:19:05
 */
public class AntispamException extends XnException {

	public AntispamException(int code) {
		super(code);
	}

	public AntispamException(int code, String message) {
		super(code,message);
	}
	public AntispamException(int code, Throwable cause) {
		super(code,cause);
	}

	public AntispamException(int code, String message, Throwable cause) {
		super(code,message, cause);
	}

}
