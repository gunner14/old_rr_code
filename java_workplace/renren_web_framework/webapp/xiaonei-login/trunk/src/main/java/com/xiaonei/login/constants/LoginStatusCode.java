package com.xiaonei.login.constants;

public enum LoginStatusCode {

	LOGIN_OK(-1, "登录成功"), FAILUNKNOWN(0, "未知错误"),

	/** 用户输入不合法 */
	FAILINPUT(1, "您输入的参数不正确"),

	/** 用户当前登录IP被封禁 */
	FAILBANIP(1 << 1, "用户当前登录IP被封禁"),

	/** 用户输入的帐号和密码不匹配 */
	FAILUSER(1 << 2, "您的用户名和密码不匹配"),

	/** 非post访问提交页面 */
	FAILNOPOST(1 << 3, "非post访问提交页面"),

	/** 帐号被封禁 */
	FAILUSERBAN(1 << 4, "帐号被停止使用"),

	/** 帐号未激活 */
	FAILUSERACTIVE(1 << 5, "帐号未激活"),

	/** 帐号已经注销 */
	FAILUSERSUICIDE(1 << 6, "帐号已经注销"),

	/** 用户不存在 */
	FAILUSERNOTEXIST(1 << 7 ,"您的用户名和密码不匹配"),

	/** 用户存在安全问题 */
	FAILNOSAFEUSER(1 << 8, "用户存在安全问题"),

	/** 用户输入验证码错误 */
	FAILVALIDATELOGIN(1 << 9, "您输入的验证码不正确"),

	/** 用户为灰名单用户 */
	FAILNOTIFYGRAYUSER(1 << 10, "用户为灰名单用户"),

	/** 用户在异常Ip登录 */
	FAILNOTIFYGRAYIP(1 << 11, "用户在异常Ip登录"),
	
	/** 用户空用户名 */
	FAILEMPTYACOUNT(1 << 13, "您的用户名和密码不匹配"),

	/** 默认错误 */
	FAILDEFAULT(1 << 12, "默认错误"),

	;

	private final int code;
	private final String description;

	private LoginStatusCode(int code, String description) {
		this.code = code;
		this.description=description;
	}

	/**
	 * @return the code
	 */
	public int getCode() {
		return code;
	}

	public String getDescription() {
		return description;
	}

}
