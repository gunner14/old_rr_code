package com.xiaonei.passport.constants;

public enum LogoutStatusCode {
	LOGOUT_OK(-1, "登录成功"), NO_LOGIN_TYPE_ERROR(0, "loginRegistrationId error"), SYSTEM_EXCEPTION(
			1, "系统异常"), PASSPORT_DESTROY_FAIL(2, "删除P票异常"), TICKET__DESTROY_FAIL(3, "删除人人T票异常"), INPUT_LOGIN_REGISTRATION_ID_ERROR(4, "没有loginRegistrationId参数"),
			OLD_PASSPORT_DESTROY_FAIL(5,"删除老的P票异常"), OLD_TICKET_DESTROY_FAIL(6,"删除老的T票异常"),FAILNOLOGINREGISTRATION(7, "没有登陆注册号");
	private int code;
	private String description;

	private LogoutStatusCode(int code, String description) {
		this.code = code;
		this.description = description;
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
