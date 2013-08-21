package com.xiaonei.reg.register.form;

import java.lang.reflect.Method;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.form.base.userganerate.IBirthday;
import com.xiaonei.reg.register.form.base.userganerate.IGender;
import com.xiaonei.reg.register.form.base.userganerate.IName;
import com.xiaonei.reg.register.form.base.userganerate.IPwd;

public class RegShortCommonForm extends BaseRegForm implements IAccType, IPwd,
		IName, IGender, IBirthday {

	private static final long serialVersionUID = 275658784949072098L;

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
	public String getBirth_day() {
		return birth_day;
	}

	@Override
	public String getBirth_month() {
		return birth_month;
	}

	@Override
	public String getBirth_year() {
		return birth_year;
	}

	@Override
	public void setBirth_day(String birthDay) {
		birth_day = birthDay;
	}

	@Override
	public void setBirth_month(String birthMonth) {
		birth_month = birthMonth;
	}

	@Override
	public void setBirth_year(String birthYear) {
		birth_year = birthYear;
	}

	@Override
	public String getNicknameId() {
		return nicknameId;
	}

	@Override
	public void setNicknameId(String nicknameId) {
		this.nicknameId = nicknameId;
	}

	public static void main(String[] args) {
		Object obj = new RegShortCommonForm();
		BaseRegForm form = (BaseRegForm) obj;
		Method[] ms = form.getClass().getMethods();
		for (Method m : ms) {
			if (StringUtils.equals(m.getName(), "getClass")) {

			} else if (StringUtils.startsWith(m.getName(), "get")) {
				String mname = StringUtils.removeStart(m.getName(), "get");
				System.out.println(mname);
				System.out.println(mname.substring(0, 1).toLowerCase()
						+ mname.substring(1));
			}
		}
		
	}
}
