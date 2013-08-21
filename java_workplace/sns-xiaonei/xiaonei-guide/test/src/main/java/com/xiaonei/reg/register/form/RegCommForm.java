/**
 *
 */
package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

/**
 * RegCommForm
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-2 - 下午04:25:54
 */
public class RegCommForm extends BaseRegForm implements IStage, IAccType, IPwd,
		IName, IGender, IHomecity, IUniversity, IMiddleschool {

	private static final long serialVersionUID = 5559987882090968946L;

	public int getStage() {
		return stage;
	}

	public void setStage(int stage) {
		this.stage = stage;
	}

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

	public String getHomecitycode() {
		return this.homecitycode;
	}

	public String getHomecityname() {
		return this.homecityname;
	}

	public String getHomeprovince() {
		return this.homeprovince;
	}

	public void setHomecitycode(String homecitycode) {
		this.homecitycode = homecitycode;
	}

	public void setHomecityname(String homecityname) {
		this.homecityname = homecityname;
	}

	public void setHomeprovince(String homeprovince) {
		this.homeprovince = homeprovince;
	}

	public String getTypeOfCourse() {
		return this.typeOfCourse;
	}

	public int getUniversityid() {
		return this.universityid;
	}

	public String getUniversityname() {
		return this.universityname;
	}

	public void setTypeOfCourse(String typeOfCourse) {
		this.typeOfCourse = typeOfCourse;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.form.base.userganerate.IUniversity#setUniversityid
	 * (int)
	 */
	public void setUniversityid(int universityid) {
		this.universityid = universityid;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.form.base.userganerate.IUniversity#setUniversityname
	 * (java.lang.String)
	 */
	public void setUniversityname(String universityname) {
		this.universityname = universityname;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.form.base.userganerate.IMiddleschool#getEnrollYear
	 * ()
	 */
	public int getEnrollYear() {
		return this.enrollYear;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getHighSchoolClass()
	 */
	public String getHighSchoolClass() {
		return this.highSchoolClass;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getHighSchoolCode()
	 */
	public String getHighSchoolCode() {
		return this.highSchoolCode;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getHighSchoolName()
	 */
	public String getHighSchoolName() {
		return this.highSchoolName;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getJuniorHighSchoolId()
	 */
	public String getJuniorHighSchoolId() {
		return this.juniorHighSchoolId;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getJuniorHighSchoolName()
	 */
	public String getJuniorHighSchoolName() {
		return this.juniorHighSchoolName;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.form.base.userganerate.IMiddleschool#getSchoolType
	 * ()
	 */
	public String getSchoolType() {
		return this.schoolType;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getTechHighSchoolId()
	 */
	public String getTechHighSchoolId() {
		return this.techHighSchoolId;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * getTechHighSchoolName()
	 */
	public String getTechHighSchoolName() {
		return this.techHighSchoolName;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.xiaonei.reg.register.form.base.userganerate.IMiddleschool#setEnrollYear
	 * (int)
	 */
	public void setEnrollYear(int enrollYear) {
		this.enrollYear = enrollYear;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * setHighSchoolClass(java.lang.String)
	 */
	public void setHighSchoolClass(String highSchoolClass) {
		this.highSchoolClass = highSchoolClass;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * setHighSchoolCode(java.lang.String)
	 */
	public void setHighSchoolCode(String highSchoolCode) {
		this.highSchoolCode = highSchoolCode;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * setHighSchoolName(java.lang.String)
	 */
	public void setHighSchoolName(String highSchoolName) {
		this.highSchoolName = highSchoolName;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
	 * setJuniorHighSchoolId(java.lang.String)
	 */
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

	public String getNicknameId() {
		return nicknameId;
	}

	public void setNicknameId(String nicknameId) {
		this.nicknameId = StringUtils.lowerCase(nicknameId);
	}

	public int getAccType() {
		return this.accType;
	}

	public void setAccType(int accType) {
		this.accType = accType;
	}

	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
				.appendSuper(super.toString())
				.append("largeUrl", this.largeUrl).append("homeprovince",
						this.homeprovince)
				.append("inviteType", this.inviteType).append(
						"juniorHighSchoolId", this.juniorHighSchoolId).append(
						"gender", this.gender).append("highSchoolClass",
						this.highSchoolClass).append("headUrl", this.headUrl)
				.append("homecitycode", this.homecitycode).append("inviterId",
						this.inviterId).append("code", this.code).append(
						"schoolType", this.schoolType).append("highSchoolCode",
						this.highSchoolCode).append("birth_month",
						this.birth_month).append("fromUrl", this.fromUrl)
				.append("regEmail", this.regEmail).append("porid", this.porid)
				.append("uuid", this.uuid).append("groupName", this.groupName)
				.append("typeOfCourse", this.typeOfCourse).append("appId",
						this.appId).append("enrollYear", this.enrollYear)
				.append("department", this.department).append("homecityname",
						this.homecityname).append("dormitory", this.dormitory)
				.append("universityid", this.universityid).append("from",
						this.from).append("stage", this.stage).append("ic",
						this.ic).append("basicControl", this.basicControl)
				.append("workplaceid", this.workplaceid).append("name",
						this.name).append("techHighSchoolId",
						this.techHighSchoolId).append("juniorHighSchoolName",
						this.juniorHighSchoolName).append("appToken",
						this.appToken).append("nicknameId", this.nicknameId)
				.append("techHighSchoolName", this.techHighSchoolName).append(
						"highSchoolName", this.highSchoolName).append("iu",
						this.iu).append("studentnumber", this.studentnumber)
				.append("audit", this.audit).append("mainUrl", this.mainUrl)
				.append("workplacename", this.workplacename).append(
						"universityname", this.universityname).append(
						"birth_year", this.birth_year).append("inviteeEmail",
						this.inviteeEmail).append("accType", this.accType)
				.append("pwd", this.pwd).append("birth_day", this.birth_day)
				.append("repwd", this.repwd).append("tinyUrl", this.tinyUrl)
				.append("channel", this.channel).toString();
	}

}
