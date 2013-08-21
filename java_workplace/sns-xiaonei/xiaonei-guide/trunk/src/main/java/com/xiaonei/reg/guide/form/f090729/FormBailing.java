package com.xiaonei.reg.guide.form.f090729;

import org.apache.struts.action.ActionForm;

public class FormBailing extends ActionForm{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1852363061040754663L;

	private String stage;
	
	private String companyname;
	
	private String universityname;
	private int universityyear;
	private int universitycode;
	private String universitydepartment;
	
	private String homeprovince;
	private String homecityname;
	private int homecitycode;
	public String getStage() {
		return stage;
	}
	public void setStage(String stage) {
		this.stage = stage;
	}
	public String getCompanyname() {
		return companyname;
	}
	public void setCompanyname(String companyname) {
		this.companyname = companyname;
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
	public int getUniversitycode() {
		return universitycode;
	}
	public void setUniversitycode(int universitycode) {
		this.universitycode = universitycode;
	}
	public String getUniversitydepartment() {
		return universitydepartment;
	}
	public void setUniversitydepartment(String universitydepartment) {
		this.universitydepartment = universitydepartment;
	}
	public String getHomeprovince() {
		return homeprovince;
	}
	public void setHomeprovince(String homeprovince) {
		this.homeprovince = homeprovince;
	}
	public String getHomecityname() {
		return homecityname;
	}
	public void setHomecityname(String homecityname) {
		this.homecityname = homecityname;
	}
	public int getHomecitycode() {
		return homecitycode;
	}
	public void setHomecitycode(int homecitycode) {
		this.homecitycode = homecitycode;
	}

	
}
