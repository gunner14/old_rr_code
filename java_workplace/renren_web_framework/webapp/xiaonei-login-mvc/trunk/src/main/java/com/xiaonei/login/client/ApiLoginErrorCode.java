package com.xiaonei.login.client;

/**
 * 登录相关的错误代码
 * 
 * @author haobo.cui@Opi-corp.com
 * 
 */
public class ApiLoginErrorCode {
	/*
	 * GENERAL ERRORS
	 */
	/** 用户帐号,密码无效 */
	// public static final ipnt USER_PASSWORD_INVALID = 1001;
	public static final int USER_PASSWORD_INVALID = 1000;

	/** 用户的ip地址封禁 */
	public static final int USER_IP_FORBIDDEN = 1002;

	/** 用户不存在 */
	public static final int USER_UNKNOW = 1003;

	/** 帐号未激活 */
	public static final int ACCOUNT_INACTIVE = 1004;

	/** 帐号被封禁 */
	public static final int ACCOUNT_FORBIDDEN = 1005;

	/** 帐号已经注销 */
	public static final int ACCOUNT_LOGINOUT = 1006;

	/** 登录服务暂时不可用，请重试 */
	public static final int LOGIN_BUSY = 1007;

	/** 无效的API_KEY */
	public static final int API_EC_PARAM_API_KEY = 1008;

	/** 无效的API_KEY */
	public static final int REQUEIRE_POST = 1009;

	/** 帐号存在安全问题 */
	public static final int ACCOUNT_UNSAFE = 1010;

	/** 输入有错误 */
	public static final int FAILINPUT = 1011;

}
