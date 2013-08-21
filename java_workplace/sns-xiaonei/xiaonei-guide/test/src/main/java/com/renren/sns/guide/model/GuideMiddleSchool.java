package com.renren.sns.guide.model;

import java.io.Serializable;

public class GuideMiddleSchool implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = -6904549088955315944L;

	private int middleSchoolId;
	
	private String middleSchoolName;

	public int getMiddleSchoolId() {
		return middleSchoolId;
	}

	public void setMiddleSchoolId(int middleSchoolId) {
		this.middleSchoolId = middleSchoolId;
	}

	public String getMiddleSchoolName() {
		return middleSchoolName;
	}

	public void setMiddleSchoolName(String middleSchoolName) {
		this.middleSchoolName = middleSchoolName;
	}
}
