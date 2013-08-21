package com.renren.sns.guide.model;

import java.io.Serializable;
import java.util.List;

public class GuideSameSchool implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = 3842385711431265162L;

	private int count;
	
	private int schoolId;
	
	private String schoolName;
	
	private String type;
	
	private String moreLink;
	
	private List<SameSchoolPeople> sspList;

	public int getCount() {
		return count;
	}

	public void setCount(int count) {
		this.count = count;
	}

	public int getSchoolId() {
		return schoolId;
	}

	public void setSchoolId(int schoolId) {
		this.schoolId = schoolId;
	}

	public String getSchoolName() {
		return schoolName;
	}

	public void setSchoolName(String schoolName) {
		this.schoolName = schoolName;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public String getMoreLink() {
		return moreLink;
	}

	public void setMoreLink(String moreLink) {
		this.moreLink = moreLink;
	}

	public List<SameSchoolPeople> getSspList() {
		return sspList;
	}

	public void setSspList(List<SameSchoolPeople> sspList) {
		this.sspList = sspList;
	}
}
