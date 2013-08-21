/**
 *
 */
package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import org.apache.commons.lang.StringUtils;

/**
 * RegEduForm
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-26 - 下午06:17:38
 */
public class RegEduForm extends BaseRegForm implements IGender, IMiddleschool,
		IName, IAccType, IPwd, IStage {

	private static final long serialVersionUID = 8593310904869755751L;

	public String getGender() {
		return this.gender;
	}

	public void setGender(String gender) {
		this.gender = gender;
	}

	public int getEnrollYear() {
		return this.enrollYear;
	}

	public String getHighSchoolClass() {
		return this.highSchoolClass;
	}

	public String getHighSchoolCode() {
		return highSchoolCode;
	}

	public String getHighSchoolName() {
		return this.highSchoolName;
	}

	public String getJuniorHighSchoolId() {
		return this.juniorHighSchoolId;
	}

	public String getJuniorHighSchoolName() {
		return this.juniorHighSchoolName;
	}

	public String getSchoolType() {
		return this.schoolType;
	}

	public String getTechHighSchoolId() {
		return this.techHighSchoolId;
	}

	public String getTechHighSchoolName() {
		return this.techHighSchoolName;
	}

	public void setEnrollYear(int enrollYear) {
		this.enrollYear = enrollYear;
	}

	public void setHighSchoolClass(String highSchoolClass) {
		this.highSchoolClass = highSchoolClass;
	}

	public void setHighSchoolCode(String highSchoolCode) {
		this.highSchoolCode = highSchoolCode;
	}

	public void setHighSchoolName(String highSchoolName) {
		this.highSchoolName = highSchoolName;
	}

	public void setJuniorHighSchoolId(String juniorHighSchoolId) {
		this.juniorHighSchoolId = juniorHighSchoolId;
	}

	public void setJuniorHighSchoolName(String juniorHighSchoolName) {
		this.juniorHighSchoolName = juniorHighSchoolName;
	}

	public void setSchoolType(String schoolType) {
		this.schoolType = schoolType;
	}

	public void setTechHighSchoolId(String techHighSchoolId) {
		this.techHighSchoolId = techHighSchoolId;
	}

	public void setTechHighSchoolName(String techHighSchoolName) {
		this.techHighSchoolName = techHighSchoolName;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = this.trimLine(name);
	}

	public String getNicknameId() {
		return this.nicknameId;
	}

	public void setNicknameId(String nicknameId) {
		this.nicknameId = StringUtils.lowerCase(nicknameId);
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

	public int getStage() {
		return this.stage;
	}

	public void setStage(int stage) {
		this.stage = stage;
	}

	@Override
	public int getAccType() {
		return this.accType;
	}

	@Override
	public String getRegEmail() {
		return this.regEmail;
	}

	@Override
	public void setAccType(int accType) {
		this.accType = accType;
	}

	@Override
	public void setRegEmail(String regEmail) {
		this.regEmail = StringUtils.lowerCase(regEmail);
	}

}