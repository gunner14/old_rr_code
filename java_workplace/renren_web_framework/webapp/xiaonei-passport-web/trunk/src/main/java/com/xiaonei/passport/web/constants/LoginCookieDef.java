package com.xiaonei.passport.web.constants;

import com.xiaonei.platform.core.opt.base.PassportManager;

public interface LoginCookieDef {

	public static final String COOKIE_PASSPORT = "passport";
	public static final String COOKIE_PASSPORT_OLD = "p";

	public static final String AUTOLOGIN = "ap";

	public static final String ICODE = "ick_login";

	public static final String EMAIL = "email";

	public static final String PASSWORD = "password";

	/** email自动填入 */
	public static final String DE = "_de";

	/** 他们要的(webpager JS FE) */
	public static final String ID = "id";

	/** 黄欢要的 */
	public static final String XNSID = "xnsid";

	/** 交叉登录的逻辑部分 (kl_id) */
	public static final String KL = "kl";

	/** old t */
	public static final String loginKey_old_ticket = PassportManager.loginKey_old_ticket;

	/** t */
	public static final String loginKey_ticket = "ticket";
	public static final String loginKey_ticket_old = "t";
	/** mobile t */
	public static final String loginKey_mticket = PassportManager.loginKey_mticket;

	public static final String TRANSFER = "transfer";
	

	/**
	 * 通讯平台使用，根据这个标志处理任务，然后清空此标志
	 */
	public static final String FIRST_LOGIN_FLAG="first_login_flag";
}
