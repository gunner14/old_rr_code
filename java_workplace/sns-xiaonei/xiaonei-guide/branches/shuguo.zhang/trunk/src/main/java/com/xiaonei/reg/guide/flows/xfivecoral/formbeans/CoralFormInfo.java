package com.xiaonei.reg.guide.flows.xfivecoral.formbeans;

import com.xiaonei.reg.guide.root.interfaces.IInfoElementaryschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoJuniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoSeniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoTechschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoUniversity;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoCurrentPlace;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoHomePlace;
import com.xiaonei.reg.guide.root.interfaces.IUserInfoStage;
import com.xiaonei.reg.guide.root.interfaces.IWorkInfoCompany;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;

public class CoralFormInfo implements IWorkInfoCompany, IInfoUniversity , IInfoSeniorhighschool,
		IInfoJuniorhighschool,IInfoTechschool,IInfoElementaryschool,IRegionInfoCurrentPlace,IRegionInfoHomePlace, IUserInfoStage {
	private int stage;

	private int companyid;
	private String companyname;

	private int universityid;
	private String universityname;
	private int universityyear;
	private int universitycode;
	private String universitydepartment;
	private String coursetype;

	private int highschoolid;
	private String highschoolname;
	private int highschoolyear;
	private int highschoolcode;
	private String highschoolclass1;
	private String highschoolclass2;
	private String highschoolclass3;
	
	private int juniorhighschoolid;
	private String juniorhighschoolname;
	private int juniorhighschoolyear;
	private int juniorhighschoolcode;
	
	private int techschoolid;
	private String techschoolname;
	private int techschoolyear;
	private int techschoolcode;
	
	private int elementaryschoolid;
	private String elementaryschoolname;
	private int elementaryschoolyear;
	private int elementaryschoolcode;
	
	private int homeregionid;
	private String homeprovince;
	private String homecityname;
	private int homecitycode;

	private int currentregionid;
	private String currentprovince;
	private String currentcityname;
	private int currentcitycode;

	public int getHomeregionid() {
		return homeregionid;
	}

	public void setHomeregionid(int homeregionid) {
		this.homeregionid = homeregionid;
	}

	public int getCurrentregionid() {
		return currentregionid;
	}

	public void setCurrentregionid(int currentregionid) {
		this.currentregionid = currentregionid;
	}

	@Override
	public int getStage() {
		return stage;
	}

	@Override
	public void setStage(int stage) {
		this.stage = stage;
	}

	public int getCompanyid() {
		return companyid;
	}

	public void setCompanyid(int companyid) {
		this.companyid = companyid;
	}

	@Override
	public String getCompanyname() {
		if(MyStringUtils.contains(companyname, "填写单位")){
			return "";
		}
		return companyname;
	}

	@Override
	public void setCompanyname(String companyname) {
		this.companyname = companyname;
	}

	@Override
	public String getUniversityname() {
		if(MyStringUtils.contains(universityname, "选择大学")){
			return "";
		}
		return universityname;
	}

	@Override
	public void setUniversityname(String universityname) {
		this.universityname = universityname;
	}

	@Override
	public int getUniversityyear() {
		return universityyear;
	}

	@Override
	public void setUniversityyear(int universityyear) {
		this.universityyear = universityyear;
	}

	@Override
	public int getUniversitycode() {
		return universitycode;
	}

	@Override
	public void setUniversitycode(int universitycode) {
		this.universitycode = universitycode;
	}

	@Override
	public String getUniversitydepartment() {
		if(MyStringUtils.contains(universitydepartment, "选择院系")){
			return "";
		}
		return universitydepartment;
	}

	@Override
	public void setUniversitydepartment(String universitydepartment) {
		this.universitydepartment = universitydepartment;
	}

	@Override
	public String getHighschoolname() {
		if(MyStringUtils.contains(highschoolname, "选择高中")){
			return "";
		}
		return highschoolname;
	}

	@Override
	public void setHighschoolname(String highschoolname) {
		this.highschoolname = highschoolname;
	}

	@Override
	public int getHighschoolyear() {
		return highschoolyear;
	}

	@Override
	public void setHighschoolyear(int highschoolyear) {
		this.highschoolyear = highschoolyear;
	}

	@Override
	public int getHighschoolcode() {
		return highschoolcode;
	}

	@Override
	public void setHighschoolcode(int highschoolcode) {
		this.highschoolcode = highschoolcode;
	}

	@Override
	public String getCoursetype() {
		if(MyStringUtils.contains(coursetype, "选择类型")){
			return "";
		}
		return coursetype;
	}

	@Override
	public void setCoursetype(String coursetype) {
		this.coursetype = coursetype;
	}

	@Override
	public String getHighschoolclass1() {
		return MyStringUtils.getNotNullString(highschoolclass1);
	}

	@Override
	public void setHighschoolclass1(String highschoolclass1) {
		this.highschoolclass1 = highschoolclass1;
	}

	@Override
	public String getHighschoolclass2() {
		return MyStringUtils.getNotNullString(highschoolclass2);
	}

	@Override
	public void setHighschoolclass2(String highschoolclass2) {
		this.highschoolclass2 = highschoolclass2;
	}

	@Override
	public String getHighschoolclass3() {
		return MyStringUtils.getNotNullString(highschoolclass3);
	}

	@Override
	public void setHighschoolclass3(String highschoolclass3) {
		this.highschoolclass3 = highschoolclass3;
	}

	
	@Override
	public String getTechschoolname() {
		return techschoolname;
	}

	@Override
	public void setTechschoolname(String techschoolname) {
		this.techschoolname = techschoolname;
	}

	@Override
	public int getTechschoolyear() {
		return techschoolyear;
	}

	@Override
	public void setTechschoolyear(int techschoolyear) {
		this.techschoolyear = techschoolyear;
	}

	@Override
	public int getTechschoolcode() {
		return techschoolcode;
	}

	@Override
	public void setTechschoolcode(int techschoolcode) {
		this.techschoolcode = techschoolcode;
	}

	@Override
	public String getElementaryschoolname() {
		if(MyStringUtils.contains(elementaryschoolname, "填写小学")){
			return "";
		}
		return elementaryschoolname;
	}

	@Override
	public void setElementaryschoolname(String elementaryschoolname) {
		this.elementaryschoolname = elementaryschoolname;
	}

	@Override
	public int getElementaryschoolyear() {
		return elementaryschoolyear;
	}

	@Override
	public void setElementaryschoolyear(int elementaryschoolyear) {
		this.elementaryschoolyear = elementaryschoolyear;
	}

	@Override
	public int getElementaryschoolcode() {
		return elementaryschoolcode;
	}

	@Override
	public void setElementaryschoolcode(int elementaryschoolcode) {
		this.elementaryschoolcode = elementaryschoolcode;
	}
	
	@Override
	public String getHomeprovince() {
		return homeprovince;
	}

	@Override
	public String getJuniorhighschoolname() {
		if(MyStringUtils.contains(juniorhighschoolname, "选择初中")){
			return "";
		}
		return juniorhighschoolname;
	}

	@Override
	public void setJuniorhighschoolname(String juniorhighschoolname) {
		this.juniorhighschoolname = juniorhighschoolname;
	}

	@Override
	public int getJuniorhighschoolyear() {
		return juniorhighschoolyear;
	}

	@Override
	public void setJuniorhighschoolyear(int juniorhighschoolyear) {
		this.juniorhighschoolyear = juniorhighschoolyear;
	}

	@Override
	public int getJuniorhighschoolcode() {
		return juniorhighschoolcode;
	}

	@Override
	public void setJuniorhighschoolcode(int juniorhighschoolcode) {
		this.juniorhighschoolcode = juniorhighschoolcode;
	}
	
	@Override
	public void setHomeprovince(String homeprovince) {
		this.homeprovince = homeprovince;
	}

	@Override
	public String getHomecityname() {
		return homecityname;
	}

	@Override
	public void setHomecityname(String homecityname) {
		this.homecityname = homecityname;
	}

	@Override
	public int getHomecitycode() {
		return homecitycode;
	}

	@Override
	public void setHomecitycode(int homecitycode) {
		this.homecitycode = homecitycode;
	}

	@Override
	public String getCurrentprovince() {
		return currentprovince;
	}

	@Override
	public void setCurrentprovince(String currentprovince) {
		this.currentprovince = currentprovince;
	}

	@Override
	public String getCurrentcityname() {
		return currentcityname;
	}

	@Override
	public void setCurrentcityname(String currentcityname) {
		this.currentcityname = currentcityname;
	}

	@Override
	public int getCurrentcitycode() {
		return currentcitycode;
	}

	@Override
	public void setCurrentcitycode(int currentcitycode) {
		this.currentcitycode = currentcitycode;
	}

	public int getUniversityid() {
		return universityid;
	}

	public void setUniversityid(int universityid) {
		this.universityid = universityid;
	}

	public int getHighschoolid() {
		return highschoolid;
	}

	public void setHighschoolid(int highschoolid) {
		this.highschoolid = highschoolid;
	}

	public int getJuniorhighschoolid() {
		return juniorhighschoolid;
	}

	public void setJuniorhighschoolid(int juniorhighschoolid) {
		this.juniorhighschoolid = juniorhighschoolid;
	}

	public int getTechschoolid() {
		return techschoolid;
	}

	public void setTechschoolid(int techschoolid) {
		this.techschoolid = techschoolid;
	}

	public int getElementaryschoolid() {
		return elementaryschoolid;
	}

	public void setElementaryschoolid(int elementaryschoolid) {
		this.elementaryschoolid = elementaryschoolid;
	}

	/**
	 * Constructs a <code>String</code> with all attributes
	 * in name = value format.
	 *
	 * @return a <code>String</code> representation 
	 * of this object.
	 */
	public String toString()
	{
	    final String TAB = " | ";
	    final String RET = "\n";
	    String retValue = "";
	    
	    retValue = "CoralFormInfo ( "
	        + "stage = " + this.stage + TAB
	        + "companyname = " + this.companyname + TAB + RET
	        + "universityname = " + this.universityname + TAB
	        + "universityyear = " + this.universityyear + TAB
	        + "universitycode = " + this.universitycode + TAB
	        + "universitydepartment = " + this.universitydepartment + TAB
	        + "coursetype = " + this.coursetype + TAB + RET
	        + "highschoolname = " + this.highschoolname + TAB
	        + "highschoolyear = " + this.highschoolyear + TAB
	        + "highschoolcode = " + this.highschoolcode + TAB
	        + "highschoolclass1 = " + this.highschoolclass1 + TAB
	        + "highschoolclass2 = " + this.highschoolclass2 + TAB
	        + "highschoolclass3 = " + this.highschoolclass3 + TAB + RET
	        + "juniorhighschoolname = " + this.juniorhighschoolname + TAB
	        + "juniorhighschoolyear = " + this.juniorhighschoolyear + TAB
	        + "juniorhighschoolcode = " + this.juniorhighschoolcode + TAB + RET
	        + "techschoolname = " + this.techschoolname + TAB
	        + "techschoolyear = " + this.techschoolyear + TAB
	        + "techschoolcode = " + this.techschoolcode + TAB + RET
	        + "elementaryschoolname = " + this.elementaryschoolname + TAB
	        + "elementaryschoolyear = " + this.elementaryschoolyear + TAB
	        + "elementaryschoolcode = " + this.elementaryschoolcode + TAB + RET
	        + "homeprovince = " + this.homeprovince + TAB
	        + "homecityname = " + this.homecityname + TAB
	        + "homecitycode = " + this.homecitycode + TAB + RET
	        + "currentprovince = " + this.currentprovince + TAB
	        + "currentcityname = " + this.currentcityname + TAB
	        + "currentcitycode = " + this.currentcitycode + TAB
	        + " )";
	
	    return retValue;
	}
	
	

}
