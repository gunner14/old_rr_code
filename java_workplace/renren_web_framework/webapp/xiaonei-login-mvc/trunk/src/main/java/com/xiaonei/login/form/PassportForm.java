package com.xiaonei.login.form;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.security.login.toolkit.encrypt.EncryptAdapter;

/**
 * PassportForm <br>
 * 
 * @author tai.wang@opi-corp.com Jan 13, 2010 - 6:43:41 PM
 */
public class PassportForm {

	/** 用户登录的目标域名 */
	private String domain = "renren.com";

	/** 提交的是否为非明文密码 */
	private boolean enpassword = false;

	/** 自动登录的判定并不只是在这里 */
	private boolean autoLogin = false;

	/** 默认值为一变量所以不能在此定义 */
	private String origURL = "";
	private String errURL = "";
	@HtmlPure
	private String email = "";

	/** 明文密码 */
	
	private String password = "";

	/** 验证码 */
	private String icode = "";
	private String p3p = "";
	private String rkey;
	private String decryptedPassword = null;

	public String getDomain() {
		return domain;
	}

	public void setDomain(String domain) {
		this.domain = domain;
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
		if (!StringUtils.isBlank(rkey)) {
			if (decryptedPassword == null) {
				PpLogger encrytLogger = LoginLoggerDef.logEncrypt;
				try {
					EncryptAdapter ea = EncryptAdapter.getInstance();
					decryptedPassword = ea.decryptForWeb(password, rkey);
					if (decryptedPassword == null) {
						// 由于解密失败后不抛出异常，只返回null，为防止多次解密，失败后置为空串
						decryptedPassword = "";
						encrytLogger.info("Decrypt password[failed]:" + email + "\t" + rkey + "\t" + password);
					} else {
						if (encrytLogger.isInfoEnabled()) {
							encrytLogger.info("Decrypt password[successed]:" + email + "\t" + rkey + "\t" + password);
						}
					}
				} catch (Exception e) {
					encrytLogger.info("Decrypt password[failed]:" + email + "\t" + rkey + "\t" + password, e);
				}
			}
			return decryptedPassword;
		}
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

	public void setErrURL(String errURL) {
		this.errURL = errURL;

	}

	public String getErrURL() {
		return errURL;
	}
	
	public String getRkey() {
		return rkey;
	}

	public void setRkey(String rkey) {
		this.rkey = rkey;
	}

	@Override
	public String toString() {
		return "PassportForm [autoLogin=" + autoLogin + ", domain=" + domain
				+ ", email=" + email + ", enpassword=" + enpassword
				+ ", errURL=" + errURL + ", icode=" + icode + ", origURL="
				+ origURL + ", p3p=" + p3p + ", password=" + password + ", rkey=" + rkey + "]";
	}

}
