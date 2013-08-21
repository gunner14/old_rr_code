package com.xiaonei.reg.guide.flows.xfive.formbeans;

import com.xiaonei.websecurity.adapter.SafeFilter;


public class FormDaxue{

	private String stage;
	
	private String universityname;
	private int universityyear;
	private int universitycode;
	private String universitydepartment;
	
	private String highschoolname;
	private int highschoolyear;
	private int highschoolcode;
	
	private String coursetype;
	
	public String getInfo(){
		return " "+stage+"."+universityname+"."+universityyear+"."+universitycode+"."+universitydepartment+"."+highschoolname+"."+highschoolyear+"."
		+highschoolcode+".";
	}
	
	public String getCoursetype() {
		return coursetype;
	}
	public void setCoursetype(String coursetype) {
		this.coursetype = SafeFilter.getInstance().getPureContent(coursetype);
	}
	public String getStage() {
		return stage;
	}
	public void setStage(String stage) {
		this.stage = stage;
	}
	public String getUniversityname() {
		return universityname;
	}
	public void setUniversityname(String universityname) {
		this.universityname = SafeFilter.getInstance().getPureContent(universityname);
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
		this.universitydepartment = SafeFilter.getInstance().getPureContent(universitydepartment);
	}
	public String getHighschoolname() {
		return highschoolname;
	}
	public void setHighschoolname(String highschoolname) {
		this.highschoolname = SafeFilter.getInstance().getPureContent(highschoolname);
	}
	public int getHighschoolyear() {
		return highschoolyear;
	}
	public void setHighschoolyear(int highschoolyear) {
		this.highschoolyear = highschoolyear;
	}
	public int getHighschoolcode() {
		return highschoolcode;
	}
	public void setHighschoolcode(int highschoolcode) {
		this.highschoolcode = highschoolcode;
	}
	
	
	
	
}
