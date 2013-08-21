package com.xiaonei.reg.guide.flows.xfive.formbeans;

import com.xiaonei.websecurity.adapter.SafeFilter;


public class FormZhongxue {
	public String getInfo(){
		return " "+stage+"."+schooltype+"."+highschoolname+"."+highschoolyear+"." +
				""+highschoolcode+"."+highschoolclass1+"."+highschoolclass2+"."+highschoolclass3+"." +
				""+juniorhighschoolname+"."+juniorhighschoolyear+"."+juniorhighschoolcode+"."+techschoolname+"." +
				""+techschoolyear+"."+techschoolcode+"."+elementaryschoolname+"."+elementaryschoolyear+"." +
				""+elementaryschoolcode;
	}
	private String stage;
	
	private String schooltype;
	
	private String highschoolname;
	private int highschoolyear;
	private int highschoolcode;
	private String highschoolclass1;
	private String highschoolclass2;

	private String highschoolclass3;

	private String juniorhighschoolname;

	private int juniorhighschoolyear;

	private int juniorhighschoolcode;

	private String techschoolname;

	private int techschoolyear;

	private int techschoolcode;

	private String elementaryschoolname;

	private int elementaryschoolyear;

	private int elementaryschoolcode;
	public String getHighschoolclass1() {
		return highschoolclass1;
	}
	public void setHighschoolclass1(String highschoolclass1) {
		this.highschoolclass1 = SafeFilter.getInstance().getPureContent(highschoolclass1);
	}
	public String getHighschoolclass2() {
		return highschoolclass2;
	}
	public void setHighschoolclass2(String highschoolclass2) {
		this.highschoolclass2 = SafeFilter.getInstance().getPureContent(highschoolclass2);
	}
	public String getHighschoolclass3() {
		return highschoolclass3;
	}
	public void setHighschoolclass3(String highschoolclass3) {
		this.highschoolclass3 = SafeFilter.getInstance().getPureContent(highschoolclass3);
	}
	public String getStage() {
		return stage;
	}
	public void setStage(String stage) {
		this.stage = stage;
	}
	public String getSchooltype() {
		return schooltype;
	}
	public void setSchooltype(String schooltype) {
		this.schooltype = SafeFilter.getInstance().getPureContent(schooltype);
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
	public String getJuniorhighschoolname() {
		return juniorhighschoolname;
	}
	public void setJuniorhighschoolname(String juniorhighschoolname) {
		this.juniorhighschoolname = SafeFilter.getInstance().getPureContent(juniorhighschoolname);
	}
	public int getJuniorhighschoolyear() {
		return juniorhighschoolyear;
	}
	public void setJuniorhighschoolyear(int juniorhighschoolyear) {
		this.juniorhighschoolyear = juniorhighschoolyear;
	}
	public int getJuniorhighschoolcode() {
		return juniorhighschoolcode;
	}
	public void setJuniorhighschoolcode(int juniorhighschoolcode) {
		this.juniorhighschoolcode = juniorhighschoolcode;
	}
	public String getTechschoolname() {
		return techschoolname;
	}
	public void setTechschoolname(String techschoolname) {
		this.techschoolname = SafeFilter.getInstance().getPureContent(techschoolname);
	}
	public int getTechschoolyear() {
		return techschoolyear;
	}
	public void setTechschoolyear(int techschoolyear) {
		this.techschoolyear = techschoolyear;
	}
	public int getTechschoolcode() {
		return techschoolcode;
	}
	public void setTechschoolcode(int techschoolcode) {
		this.techschoolcode = techschoolcode;
	}
	public String getElementaryschoolname() {
		return elementaryschoolname;
	}
	public void setElementaryschoolname(String elementaryschoolname) {
		this.elementaryschoolname = SafeFilter.getInstance().getPureContent(elementaryschoolname);
	}
	public int getElementaryschoolyear() {
		return elementaryschoolyear;
	}
	public void setElementaryschoolyear(int elementaryschoolyear) {
		this.elementaryschoolyear = elementaryschoolyear;
	}
	public int getElementaryschoolcode() {
		return elementaryschoolcode;
	}
	public void setElementaryschoolcode(int elementaryschoolcode) {
		this.elementaryschoolcode = elementaryschoolcode;
	}
	
}
