package com.xiaonei.passport.logger;



public interface LoginLoggerDef {

	public static final String LOGIN_LOGGER_NAME = "passportLog";
	public static final String loginLogPentter = "status:%s;account:%s;desc:%s;email:%s;password:%s;loginRegistrationId:%s";
	// public static final PpLogger logoutTrace =
	// PassportLoggerFactory.getLogger("logoutTrace", true);

	public static final PpLogger login = PassportLoggerFactory.getLogger(
			"login", true);

	/** 灰名单 */
	public static final PpLogger loginGrayList = PassportLoggerFactory
			.getLogger("loginGrayList", true);

	/** 安全登录 */
	public static final PpLogger loginSafe = PassportLoggerFactory.getLogger(
			"loginSafe", true);

	/** 旧登录项目 */
	public static final PpLogger loginHttp = PassportLoggerFactory.getLogger(
			"loginHttp", true);

	/** 客户端登录项目 */
	public static final PpLogger loginClient = PassportLoggerFactory.getLogger(
			"loginClient", true);

	/** 登录失败注册流程开关 */
	public static final PpLogger loginFailToRegSwitch = PassportLoggerFactory
			.getLogger("loginFailToRegSwitch", true);

	public static final PpLogger logout = PassportLoggerFactory.getLogger(
			"logout", true);

	public static final PpLogger tstat = PassportLoggerFactory.getLogger(
			"Stat", false);

	/** 错误密码统计 */
	public static final PpLogger twrongPW = PassportLoggerFactory.getLogger(
			"WrongLoginStat", true);

	public static final PpLogger feedTask = PassportLoggerFactory.getLogger(
			"FeedTask", false);

	public static final PpLogger flyerTask = PassportLoggerFactory.getLogger(
			"FlyerTaskSet", false);

	public static final PpLogger logTask = PassportLoggerFactory.getLogger(
			"LogTaskSet", false);

	/** time cost */
	public static final PpLogger logTimeCost = PassportLoggerFactory.getLogger(
			"timeCost", false);
	
	/** 密码统计 */
	public static final PpLogger logMima = PassportLoggerFactory.getLogger(
			"mima", false);
	
	/** 完善大学资料 */
	public static final PpLogger logCompleteUnivInfo = PassportLoggerFactory.getLogger(
			"cui", true);
}
