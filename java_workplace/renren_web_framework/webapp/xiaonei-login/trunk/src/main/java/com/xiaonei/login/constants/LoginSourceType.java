package com.xiaonei.login.constants;

import com.xiaonei.aegis.topusing.AegisLoginConstants;

public enum LoginSourceType {
	DaQin(AegisLoginConstants.LoginType.WEBLOGIN_DaQin.ordinal()), Web(AegisLoginConstants.LoginType.WEBLOGIN.ordinal()), Client(
			AegisLoginConstants.LoginType.WEBLOGIN_CLIENT.ordinal()), Platform(
			AegisLoginConstants.LoginType.WEBLOGIN_APP.ordinal()), WEBLOGIN_56(AegisLoginConstants.LoginType.WEBLOGIN_56.ordinal()),
			WEBLOGIN_FENGCHE(AegisLoginConstants.LoginType.WEBLOGIN_FENGCHE.ordinal());
			;
	private int value;

	private LoginSourceType(int value) {
		this.value = value;
	}

	public int getValue() {
		return this.value;
	}
}
