package com.xiaonei.reg.guide.form;

import org.apache.struts.action.ActionForm;

public class Fillinfo4399HsForm extends ActionForm {

    private static final long serialVersionUID = 1231741457048496446L;

    /**
     * 高中
     */
    public static final int HIGH_SCHOOL_TYPE = 0;
    /**
     * 初中
     */
    public static final int JUNIOR_SCHOOL_TYPE = 1;
    /**
     * 技校
     */
    public static final int TECH_SCHOOL_TYPE = 2;

    /**
     * 学校类型
     */
    private int schoolType;
    /**
     * 高中名称
     */
    private String highSchoolName;
    /**
     * 高中 编号
     */
    private int highSchoolCode;
    /**
     * 技校名称
     */
    private String techSchoolName;
    /**
     * 技校 编号
     */
    private int techSchoolCode;
    /**
     * 初中
     */
    private String juniorSchoolName;
    /**
     * 初中code
     */
    private int juniorSchoolCode;
    /**
     * 入学年
     */
    private int enrollYear;

    public int getSchoolType() {
        return schoolType;
    }

    public void setSchoolType(int schoolType) {
        this.schoolType = schoolType;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

    public int getHighSchoolCode() {
        return highSchoolCode;
    }

    public void setHighSchoolCode(int highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    public String getTechSchoolName() {
        return techSchoolName;
    }

    public void setTechSchoolName(String techSchoolName) {
        this.techSchoolName = techSchoolName;
    }

    public int getTechSchoolCode() {
        return techSchoolCode;
    }

    public void setTechSchoolCode(int techSchoolCode) {
        this.techSchoolCode = techSchoolCode;
    }

    public String getJuniorSchoolName() {
        return juniorSchoolName;
    }

    public void setJuniorSchoolName(String juniorSchoolName) {
        this.juniorSchoolName = juniorSchoolName;
    }

    public int getJuniorSchoolCode() {
        return juniorSchoolCode;
    }

    public void setJuniorSchoolCode(int juniorSchoolCode) {
        this.juniorSchoolCode = juniorSchoolCode;
    }

    public int getEnrollYear() {
        return enrollYear;
    }

    public void setEnrollYear(int enrollYear) {
		this.enrollYear = enrollYear;
	}
}
