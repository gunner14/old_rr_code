/**
 * 
 */
package com.xiaonei.login.logic.validator.enumeration;

import com.xiaonei.login.logic.validator.DomainValidator;
import com.xiaonei.login.logic.validator.EmailValidator;
import com.xiaonei.login.logic.validator.ErrUrlValidator;
import com.xiaonei.login.logic.validator.OrigUrlValidator;
import com.xiaonei.login.logic.validator.PasswordValidator;
import com.xiaonei.login.logic.validator.PpParamValidator;

/**
 * ValidatorEnum <br>
 * 
 * @author tai.wang@opi-corp.com Jan 15, 2010 - 3:12:30 PM
 */
public enum ValidatorEnum {
	/** 用户登录的目标域名 */
	domain(DomainValidator.class),

	/** 提交的是否为非明文密码 */
	enpassword(null),

	/** 自动登录的判定并不只是在这里 */
	autologin(null),

	/** 默认值为一变量所以不能在此定义 */
	origURL(OrigUrlValidator.class),

	errURL(ErrUrlValidator.class),

	email(EmailValidator.class),

	/** 明文密码 */
	password(PasswordValidator.class),

	/** 验证码 */ 
	icode(null),

	p3p(null);

	/**
	 * @param validatorClazz
	 *            对应的验证类
	 * 
	 * @author tai.wang@opi-corp.com Jan 21, 2010 - 5:01:05 PM
	 */
	private ValidatorEnum(Class<? extends PpParamValidator> validatorClazz) {
		this.setValidatorClazz(validatorClazz);
	}

	public void setValidatorClazz(
			Class<? extends PpParamValidator> validatorClazz) {
		this.validatorClazz = validatorClazz;
	}

	public Class<? extends PpParamValidator> getValidatorClazz() {
		return validatorClazz;
	}

	private Class<? extends PpParamValidator> validatorClazz;

}
