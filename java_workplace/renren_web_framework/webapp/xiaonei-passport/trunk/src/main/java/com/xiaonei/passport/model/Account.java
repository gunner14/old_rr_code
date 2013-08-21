package com.xiaonei.passport.model;

import java.io.Serializable;

import com.xiaonei.passport.constants.LoginStatusCode;

public class Account implements Serializable {
	private int id = -1;
	private LoginStatusCode code;
	private String account;
	private String domain;
	private int status;
	private int realId = -1;
	private int safeStatus;

	public Account(long id, String account, String domain, int status, int realId, int safeStatus) {
		this.id = Long.valueOf(id).intValue();
		this.account = account;
		this.domain = domain;
		this.status = status;
		this.realId = realId;
		this.safeStatus=safeStatus;
		code = createCode(status, this.id, realId);
	}

	private LoginStatusCode createCode(int success, int id, int realId) {
		// 输入密码错误
		if (success == -2) {
			return LoginStatusCode.FAILUSER;
		}
		// 输入用户不存在
		if (success == -1) {
			return LoginStatusCode.FAILUSERNOTEXIST;
		}
		if (success < 0) {
			return LoginStatusCode.FAILUSERNOTEXIST;
		}
		if (id < 0|| realId<0) {
			return LoginStatusCode.FAILUSERNOTEXIST;
		}
		return LoginStatusCode.LOGIN_OK;
	}

	public Account(LoginStatusCode code, String account) {
		this.code = code;
		this.account = account;
	}

	public LoginStatusCode getCode() {
		return code;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getRealId() {
		return realId;
	}

	public void setRealId(int realId) {
		this.realId = realId;
	}

	public String getAccount() {
		return account;
	}

	public String getDomain() {
		return domain;
	}

	public int getStatus() {
		return status;
	}

	@Override
	public String toString() {
		StringBuffer buff = new StringBuffer();
		buff.append("id=").append(id).append(";");
		buff.append("account=").append(account).append(";");
		buff.append("success=").append(code.getCode()).append(";");
		buff.append("domain=").append(domain).append(";");
		buff.append("status=").append(status).append(";");
		buff.append("realId=").append(realId).append(";");
		buff.append("safeStatus=").append(safeStatus).append(";");
		return buff.toString();
	}
}