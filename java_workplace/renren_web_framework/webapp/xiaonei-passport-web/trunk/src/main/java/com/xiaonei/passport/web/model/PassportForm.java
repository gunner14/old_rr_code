package com.xiaonei.passport.web.model;

/**
 * PassportForm <br>
 * 
 * @author tai.wang@opi-corp.com Jan 13, 2010 - 6:43:41 PM
 */
public class PassportForm {

	/** 提交的是否为非明文密码 */
	private boolean enpassword = false;

	/** 自动登录的判定并不只是在这里 */
	private boolean autoLogin = false;

	/** 默认值为一变量所以不能在此定义 */
	private String origURL = "";

	private String email = "";

	/** 明文密码 */
	private String password = "";

	/** 验证码 */
	private String icode = "";

	private String p3p = "";
	
	private int key_id = 1;
	
	private String captcha_type = "web_login";
	
	private String domain="renren.com";
	
	private String errorUrl;

	public String getCaptchaType() {
		return captcha_type;
	}

	public void setCaptchaType(String captchaType) {
		this.captcha_type = captchaType;
	}

	public boolean isEnpassword() {
		return enpassword;
	}

	public void setEnpassword(boolean enpassword) {
		this.enpassword = enpassword;
	}

	public boolean isAutoLogin() {
		return autoLogin;
	}

	public void setAutoLogin(boolean autoLogin) {
		this.autoLogin = autoLogin;
	}

	public String getOrigURL() {
		return origURL;
	}

	public void setOrigURL(String origURL) {
		this.origURL = origURL;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getIcode() {
		return icode;
	}

	public void setIcode(String icode) {
		this.icode = icode;
	}

	public String getP3p() {
		return p3p;
	}

	public void setP3p(String p3p) {
		this.p3p = p3p;
	}


	public int getLoginRegistrationId() {
		return key_id;
	}

	public void setLoginRegistrationId(int loginRegistrationId) {
		this.key_id = loginRegistrationId;
	}

	public String getDomain() {
		return domain;
	}

	public void setDomain(String domain) {
		this.domain = domain;
	}

	public String getErrorUrl() {
		return errorUrl;
	}

	public void setErrorUrl(String errorUrl) {
		this.errorUrl = errorUrl;
	}

	@Override
	public String toString() {
		return "PassportForm [autoLogin=" + autoLogin 
				+ ", email=" + email + ", enpassword=" + enpassword+", errorUrl="+errorUrl
				+ ", icode=" + icode + ", origURL="
				+ origURL + ", p3p=" + p3p + ", password=" + password +  ", loginRegistrationId=" + key_id +",captchaType="+captcha_type+",domain="+domain+"]";
	}

}
