/**
 *
 */
package com.xiaonei.reg.guide.form;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;
import org.apache.struts.action.ActionForm;

import com.xiaonei.websecurity.adapter.SafeFilter;

import java.io.Serializable;

/**
 * MobileDXFillInfoForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:33:57
 */
public class MobileDXFillInfoForm extends ActionForm implements Serializable {

    private static final long serialVersionUID = 2122811358055061348L;

    /**
     * 性别
     */
    private String gender;
    /**
     * 类型
     */
    private String typeOfCourse;
    /**
     * 大学
     */
    private String univName;
    /**
     * 大学
     */
    private int univId;
    /**
     * 院系
     */
    private String department;
    /**
     * 大学 入学年
     */
    private int univEnrollyear;
    /**
     * 高中
     */
    private String highSchoolName;
    /**
     * 高中
     */
    private int highSchoolCode;
    /**
     * 高中 入学年
     */
    private int highSchoolEnrollYear;

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = SafeFilter.getInstance().getPureContent(gender);
    }

    public String getTypeOfCourse() {
        return typeOfCourse;
    }

    public void setTypeOfCourse(String typeOfCourse) {
        this.typeOfCourse = SafeFilter.getInstance().getPureContent(typeOfCourse);
    }

    public String getUnivName() {
        return univName;
    }

    public void setUnivName(String univName) {
        this.univName = SafeFilter.getInstance().getPureContent(univName);
    }

    public int getUnivId() {
        return univId;
    }

    public void setUnivId(int univId) {
        this.univId = univId;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = SafeFilter.getInstance().getPureContent(department);
    }

    public int getUnivEnrollyear() {
        return univEnrollyear;
    }

    public void setUnivEnrollyear(int univEnrollyear) {
        this.univEnrollyear = univEnrollyear;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = SafeFilter.getInstance().getPureContent(highSchoolName);
    }

    public int getHighSchoolCode() {
        return highSchoolCode;
    }

    public void setHighSchoolCode(int highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    public int getHighSchoolEnrollYear() {
        return highSchoolEnrollYear;
    }

    public void setHighSchoolEnrollYear(int highSchoolEnrollYear) {
        this.highSchoolEnrollYear = highSchoolEnrollYear;
    }

    /**
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE).appendSuper(super.toString()).append("univEnrollyear", this.univEnrollyear).append(
                "multipartRequestHandler", this.multipartRequestHandler).append("highSchoolCode", this.highSchoolCode)
                .append("typeOfCourse", this.typeOfCourse).append("highSchoolName", this.highSchoolName).append("highSchoolEnrollYear",
                        this.highSchoolEnrollYear).append("department", this.department).append("gender", this.gender).append("univName", this.univName).append(
                        "univId", this.univId).append("servlet", this.servlet).toString();
	}
	
}
