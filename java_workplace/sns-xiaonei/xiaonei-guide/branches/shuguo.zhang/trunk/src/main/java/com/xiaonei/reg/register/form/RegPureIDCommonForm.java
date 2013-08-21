package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import org.apache.commons.lang.StringUtils;

public class RegPureIDCommonForm extends BaseRegForm implements IAccType,
		IPwd, IName, IGender {

	private static final long serialVersionUID = 4960750371474662433L;

	public String getRegEmail() {
		return this.regEmail;
	}

	public void setRegEmail(String regEmail) {
		this.regEmail = StringUtils.lowerCase(regEmail);
	}

	public String getPwd() {
		return this.pwd;
	}

	public String getRepwd() {
		return this.repwd;
	}

	public void setPwd(String pwd) {
		this.pwd = pwd;
	}

	public void setRepwd(String repwd) {
		this.repwd = repwd;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = this.trimLine(name);
	}

	public String getGender() {
		return this.gender;
	}

	public void setGender(String gender) {
		this.gender = gender;
	}

	public int getAccType() {
		return this.accType;
	}

	public void setAccType(int accType) {
		this.accType = accType;
	}

	@Override
	public String getNicknameId() {
		return this.nicknameId;
	}

	@Override
	public void setNicknameId(String nicknameId) {
		this.nicknameId = nicknameId;
	}

}
