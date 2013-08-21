package com.xiaonei.reg.guide.flows.xfive.formbeans;

import com.xiaonei.websecurity.adapter.SafeFilter;


public class VKFormDaxue{

	private String stage;
	
	private String universityname;
	private int universityyear;
	private int universitycode;
	private String universitydepartment;
	
	private String highschoolname;
	private int highschoolyear;
	private int highschoolcode;
	
	private String coursetype;
	
	
	private String highschoolclass1;
	private String highschoolclass2;

	private String highschoolclass3;
	
	public String getInfo(){
		return " "+stage+"."+universityname+"."+universityyear+"."+universitycode+"."+universitydepartment+"."+highschoolname+"."+highschoolyear+"."
		+highschoolcode+"."+highschoolclass1+"."+highschoolclass2+"."+highschoolclass3+".";
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

	public String getHighschoolclass1() {
		return highschoolclass1;
	}

	public void setHighschoolclass1(String highschoolclass1) {
		this.highschoolclass1 = highschoolclass1;
	}

	public String getHighschoolclass2() {
		return highschoolclass2;
	}

	public void setHighschoolclass2(String highschoolclass2) {
		this.highschoolclass2 = highschoolclass2;
	}

	public String getHighschoolclass3() {
		return highschoolclass3;
	}

	public void setHighschoolclass3(String highschoolclass3) {
		this.highschoolclass3 = highschoolclass3;
	}
}
