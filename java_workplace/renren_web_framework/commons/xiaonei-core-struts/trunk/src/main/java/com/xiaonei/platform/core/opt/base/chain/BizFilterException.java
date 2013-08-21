package com.xiaonei.platform.core.opt.base.chain;

import com.xiaonei.platform.framework.exception.XnException;

/**
 * BizFilter中抛出的异常。
 * 
 * @author Li Weibo
 * @since 2008-12-28 上午12:12:49
 */
public class BizFilterException extends XnException {
	
	private static final long serialVersionUID = -1936420275932007947L;
	
	public static final int CODE_UNKNOWN_EXCEPTION = 0;	//未知异常
	
	public static final int CODE_BAD_USER_STATUS = 10;	//用户状态错误，未激活或者被封禁等
	
	public static final int CODE_USER_STATUS_ACTIVATE_VERIFIED = 11;	//User.statusActivateVerified
	
	public static final int CODE_NEED_TO_LOGIN = 20;	//需要登录
	
	public static final int CODE_INVALID_POST_REFERER = 30;	//POST请求的referer不正确
	
    public static final int CODE_SAFE_CENTER = 40;    //需要
	
	public BizFilterException(int code) {
		super(code);
	}
	public BizFilterException(int code, String message) {
		super(code, message);
	}
	public BizFilterException(int code, String message, Throwable cause) {
		super(code, message, cause);
	}
	public BizFilterException(int code, Throwable cause) {
		super(code, cause);
	}
}
