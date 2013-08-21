package com.xiaonei.passport.constants;

public enum LoginConfigureKey {
	LOGIC_BEFORE_LOGIN("logicBeforeLogic"), LOGIC_AFTER_LOGIN("logicAfterLogic"), LOGIC_FAIL_LOGIN(
			"logicFailLogic"), TRANSFER_LOGIC_BEFORE_LOGIN(
			"transferLogicBeforeLogic"), TRANSFER_LOGIC_AFTER_LOGIN(
			"transferLogicAfterLogic"), TRANSFER_LOGIC_FAIL_LOGIN(
			"transferLogicFailLogic"),SWITCH_LOGIC_BEFORE_LOGIN(
			"switchLogicBeforeLogic"), SWITCH_LOGIC_AFTER_LOGIN(
			"switchLogicAfterLogic"), SWITCH_LOGIC_FAIL_LOGIN(
			"switchLogicFailLogic"),LOGIN_TYPE("loginType"), LOGIN_SOURCE_TYPE(
			"loginSourceType"), WEB_LOGIN_BEFORE_LOGIN_EVENT(
			"webLoginBeforeLoginEvent"), WEB_LOGIN_FAIL_LOGIN_EVENT(
			"webLoginFailLoginEvent"), WEB_LOGIN_SUCCESS_LOGIN_EVENT(
			"webLoginSuccessLoginEvent"), WEB_GET_LOGIN_BEFORE_LOGIN_EVENT(
			"webGetLoginBeforeLoginEvent"), WEB_GET_LOGIN_SUCCESS_LOGIN_EVENT(
			"webGetLoginSuccessLoginEvent"), DOMAIN("domain"), TASK_LAST_IP_TASK_LOG_TYPE(
			"taskLastIpTaskLogType"), TASK_USER_SCORE_TASK_LOGIN_TYPE(
			"taskUserScoreTask"), WEB_WIDGET("webWidget"), OLD_PASSPORT_ADAPT(
			"oldPassportAdapt"), VALIDATION_DOMAIN("validationDomain"), VALIDATION_STATIC_URL("validatinStaticUrl"),
			FORBIDDEN_UA("forbiddenUserAgent"), ONLY_NEW_PASSPORT("onlyNewPassport"), USE_VM_SERVICE("useVmService"),
			REDIRECT_LOGIN_BEFORE_LOGIN_EVENT("RedirectLoginBeforeLoginEvent"),
			REDIRECT_LOGIN_FAIL_LOGIN_EVENT("RedirectLoginFailLoginEvent"),
			GET_REDIRECT_LOGIN_BEFORE_LOGIN_EVENT("GetRedirectLoginBeforeLoginEvent"),
			GET_PAGE_LOGIN_BEFORE_LOGIN_EVENT("GetPageLoginBeforeLoginEvent"),
			GET_PAGE_LOGIN_FAIL_LOGIN_EVENT("GetPageLoginFailLoginEvent"),
			GET_WEB_LOGIN_FAIL_LOGIN_EVENT("GetWebLoginFailLoginEvent"),
			WEB_SWITCH_SUCCESS_LOGIN_EVENT("webSwitchSuccessLoginEvent"),
			WEB_SWITCH_FAIL_LOGIN_EVENT("webSwitchFailLoginEvent");

	// 任务名称
	private final String code;

	private LoginConfigureKey(String code) {
		this.code = code;
	}

	/**
	 * @return the code
	 */
	public String getKey() {
		return code;
	}
}
