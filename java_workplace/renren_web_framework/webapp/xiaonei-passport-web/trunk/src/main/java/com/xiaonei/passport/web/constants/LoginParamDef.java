package com.xiaonei.passport.web.constants;

public interface LoginParamDef {

	public static final String PARAM_AUTOLOGIN = "autoLogin";

	public static final String PARAM_ORIGURL = "origURL";

	public static final String PARAM_FROM = "from";

	public static final String PARAM_EMAIL = "email";

	public static final String PARAM_ENPWD = "enpassword";

	public static final String PARAM_DOMAIN = "domain";

	public static final String PARAM_PASSWORD = "password";

	public static final String ATTRIBUTE_ORIGURL = PARAM_ORIGURL;
	
	public static final String ATTRIBUTE_PUZZLE_ORIGURL = "puzzle_origURL";

	public static final String ATTRIBUTE_LOGIN_CROSS = "loginCross";

	public static final String ATTRIBUTE_LOGINTYPE_PRE = "logintype";

	public static final String ATTRIBUTE_USER_ID = "user_id";

	public static final String ATTRIBUTE_AUTOLOGIN = LoginCookieDef.AUTOLOGIN;

	public static final String ATTRIBUTE_WEAK_PASSWORD_WARNING = LoginMemcachedKeyDef.WEAK_PASSWORD_WARNING;

	public static final String ATTRIBUTE_DE = LoginCookieDef.DE;

	public static final String ATTRIBUTE_HOSTID = "hostid";

	public static final String ATTRIBUTE_NEED_VALIDATE = "needValidate";
	
	public static final String ATTRIBUTE_ACCOUNT = "account";

	public static final String PARAM_ISPLOGIN = "isplogin";
	
	public static final String ATTRIBUTE_LOGIN_STATUS_CODE = "LoginStatusCode";
	
	public static final String ATTRIBUTE_ENCODE_PASSWORD = "encodePassword";
	
	public static final String ATTRIBUTE_EMAIL_LOGIN_URL = "emailLoginUrl";
	
	public static final String ATTRIBUTE_LOGIN_SOURCE = "loginSource";
	
}
