/**
 * 
 */
package com.xiaonei.reg.guide.form;

import org.apache.struts.action.ActionForm;

/**
 * @author wangtai
 * 
 */
public class NoStageFillInfoForm extends ActionForm {

	private static final long serialVersionUID = 5631504739116813263L;

	private int porId;
	private String tinyUrl;
	private String headUrl;
	private String mainUrl;
	private String largeUrl;
	private String isskip;
	private String homecityName;
	private String highSchoolName;
	private int highschoolyear;
	private String universityname;
	private int universityyear;
	private String department;
	private String company;
	private String homeprovince;
	private int homecityCode;
	private int universityId;
	private int schoolType;
	private int highSchoolCode;
	private int juniorHighSchoolId;
	private String juniorHighSchoolName;
	private int techHighSchoolId;
	private String techHighSchoolName;

	public int getSchoolType() {
		return schoolType;
	}

	public void setSchoolType(int schoolType) {
		this.schoolType = schoolType;
	}

	public int getHighSchoolCode() {
		return highSchoolCode;
	}

	public void setHighSchoolCode(int highSchoolCode) {
		this.highSchoolCode = highSchoolCode;
	}

	public int getJuniorHighSchoolId() {
		return juniorHighSchoolId;
	}

	public void setJuniorHighSchoolId(int juniorHighSchoolId) {
		this.juniorHighSchoolId = juniorHighSchoolId;
	}

	public String getJuniorHighSchoolName() {
		return juniorHighSchoolName;
	}

	public void setJuniorHighSchoolName(String juniorHighSchoolName) {
		this.juniorHighSchoolName = juniorHighSchoolName;
	}

	public int getTechHighSchoolId() {
		return techHighSchoolId;
	}

	public void setTechHighSchoolId(int techHighSchoolId) {
		this.techHighSchoolId = techHighSchoolId;
	}

	public String getTechHighSchoolName() {
		return techHighSchoolName;
	}

	public void setTechHighSchoolName(String techHighSchoolName) {
		this.techHighSchoolName = techHighSchoolName;
	}

	public String getTinyUrl() {
		return tinyUrl;
	}

	public void setTinyUrl(String tinyUrl) {
		this.tinyUrl = tinyUrl;
	}

	public String getHeadUrl() {
		return headUrl;
	}

	public void setHeadUrl(String headUrl) {
		this.headUrl = headUrl;
	}

	public String getMainUrl() {
		return mainUrl;
	}

	public void setMainUrl(String mainUrl) {
		this.mainUrl = mainUrl;
	}

	public String getLargeUrl() {
		return largeUrl;
	}

	public void setLargeUrl(String largeUrl) {
		this.largeUrl = largeUrl;
	}

	public String getIsskip() {
		return isskip;
	}

	public void setIsskip(String isskip) {
		this.isskip = isskip;
	}

	public String getHomecityName() {
		return homecityName;
	}

	public void setHomecityName(String homecityName) {
		this.homecityName = homecityName;
	}

	public String getHighSchoolName() {
		return highSchoolName;
	}

	public void setHighSchoolName(String highSchoolName) {
		this.highSchoolName = highSchoolName;
	}

	public int getHighschoolyear() {
		return highschoolyear;
	}

	public void setHighschoolyear(int highschoolyear) {
		this.highschoolyear = highschoolyear;
	}

	public String getUniversityname() {
		return universityname;
	}

	public void setUniversityname(String universityname) {
		this.universityname = universityname;
	}

	public int getUniversityyear() {
		return universityyear;
	}

	public void setUniversityyear(int universityyear) {
		this.universityyear = universityyear;
	}

	public String getDepartment() {
		return department;
	}

	public void setDepartment(String department) {
		this.department = department;
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String company) {
		this.company = company;
	}

	public String getHomeprovince() {
		return homeprovince;
	}

	public void setHomeprovince(String homeprovince) {
		this.homeprovince = homeprovince;
	}

	public int getHomecityCode() {
		return homecityCode;
	}

	public void setHomecityCode(int homecityCode) {
		this.homecityCode = homecityCode;
	}

	public void setPorId(int porId) {
		this.porId = porId;
	}

	public int getPorId() {
		return porId;
	}

	public int getUniversityId() {
		return universityId;
	}

	public void setUniversityId(int universityId) {
		this.universityId = universityId;
	}

}
