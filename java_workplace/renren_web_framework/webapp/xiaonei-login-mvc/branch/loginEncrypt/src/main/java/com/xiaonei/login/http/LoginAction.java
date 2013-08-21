package com.xiaonei.login.http;

import org.apache.struts.action.Action;

import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.logger.PpLogger;

public abstract class LoginAction extends Action {

	protected static final PpLogger _Tstat = LoginLoggerDef.tstat;

	public static final PpLogger _TwrongPW = LoginLoggerDef.twrongPW;

	protected static final PpLogger _log = LoginLoggerDef.loginHttp;

	// 用户提交的几个主要的参数
	public static final String PARAM_AUTOLOGIN = "autoLogin";

	public static final String PARAM_ORIGURL = "origURL";

	public static final String PARAM_FROM = "from";

	public static final String PARAM_EMAIL = "email";

	public static final String PARAM_ENPWD = "enpassword";

	public static final String PARAM_DOMAIN = "domain";

	public static final String PARAM_PASSWORD = "password";

	public static final String COOKIE_PASSPORT = "p";

	public static final String PASSPORT_DOMAIN = Globals.PASSPORT_DOMAIN;

	public static final String PASSPORT_3RD_DOMAIN = PASSPORT_DOMAIN;

	public static final String LOGIN_CROSS = "loginCross";

	public static final String AUTOLOGIN = "ap";

	public static final String USER_ID = "user_id";

	public static final String LOGINTYPE_PRE = "logintype";

	public static final String ACCOUNT = "account";

	public static final String WEAKPASSWORDWARNING = "login_weakpassword";

}
