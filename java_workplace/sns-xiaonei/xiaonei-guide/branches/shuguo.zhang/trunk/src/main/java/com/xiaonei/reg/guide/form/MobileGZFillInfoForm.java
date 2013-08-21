/**
 *
 */
package com.xiaonei.reg.guide.form;

import com.xiaonei.reg.common.constants.IMiddleSchoolType;
import com.xiaonei.websecurity.adapter.SafeFilter;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;
import org.apache.struts.action.ActionForm;

import java.io.Serializable;

/**
 * MobileGZFillInfoForm 
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午06:25:52
 *
 */

/**
 * MobileGZFillInfoForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-10 - 下午05:05:45
 */
public class MobileGZFillInfoForm extends ActionForm implements Serializable {

    private static final long serialVersionUID = 2815937908434446982L;

    /**
     * 高中
     */
    public static final int HIGH_SCHOOL_TYPE = IMiddleSchoolType.HIGH_SCHOOL_TYPE;
    /**
     * 初中
     */
    public static final int JUNIOR_SCHOOL_TYPE = IMiddleSchoolType.JUNIOR_SCHOOL_TYPE;
    /**
     * 技校
     */
    public static final int TECH_SCHOOL_TYPE = IMiddleSchoolType.TECH_SCHOOL_TYPE;

    /**
     * 性别
     */
    private String gender;
    /**
     * 家乡
     */
    private String hometownProvince;
    /**
     * 家乡 编号
     */
    private String hometownCity;
    private String hometownCityCode;

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
     * 高中入学年
     */
    private int highSchoolEnrollYear;
    /**
     * 技校名称
     */
    private String techSchoolName;
    /**
     * 技校 编号
     */
    private int techSchoolCode;
    /**
     * 技校入学年
     */
    private int techSchoolEnrollYear;
    /**
     * 初中
     */
    private String juniorSchoolName;
    /**
     * 初中code
     */
    private int juniorSchoolCode;
    /**
     * 初中入学年
     */
    private int juniorSchoolEnrollYear;
    /**
     * 小学
     */
    private String elementarySchoolName;
    /**
     * 小学入学年
     */
    private int elementarySchoolEnrollYear;

    /**
     * 头像相关
     */
    private String headUrl;
    private String largeUrl;
    private String mainUrl;
    private String tinyUrl;

    public static void main(String[] args) {
	}
    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = SafeFilter.getInstance().getPureContent(headUrl);
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = SafeFilter.getInstance().getPureContent(largeUrl);
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        this.mainUrl = SafeFilter.getInstance().getPureContent(mainUrl);
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = SafeFilter.getInstance().getPureContent(tinyUrl);
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = SafeFilter.getInstance().getPureContent(gender);
    }

    public String getHometownProvince() {
        return hometownProvince;
    }

    public void setHometownProvince(String hometownProvince) {
        this.hometownProvince = SafeFilter.getInstance().getPureContent(hometownProvince);
    }

    public String getHometownCity() {
        return hometownCity;
    }

    public void setHometownCity(String hometownCity) {
        this.hometownCity = SafeFilter.getInstance().getPureContent(hometownCity);
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

    public String getJuniorSchoolName() {
        return juniorSchoolName;
    }

    public void setJuniorSchoolName(String juniorSchoolName) {
        this.juniorSchoolName = SafeFilter.getInstance().getPureContent(juniorSchoolName);
    }

    public int getJuniorSchoolCode() {
        return juniorSchoolCode;
    }

    public void setJuniorSchoolCode(int juniorSchoolCode) {
        this.juniorSchoolCode = juniorSchoolCode;
    }

    public int getJuniorSchoolEnrollYear() {
        return juniorSchoolEnrollYear;
    }

    public void setJuniorSchoolEnrollYear(int juniorSchoolEnrollYear) {
        this.juniorSchoolEnrollYear = juniorSchoolEnrollYear;
    }

    public String getElementarySchoolName() {
        return elementarySchoolName;
    }

    public void setElementarySchoolName(String elementarySchoolName) {
        this.elementarySchoolName = SafeFilter.getInstance().getPureContent(elementarySchoolName);
    }

    public int getElementarySchoolEnrollYear() {
        return elementarySchoolEnrollYear;
    }

    public void setElementarySchoolEnrollYear(int elementarySchoolEnrollYear) {
        this.elementarySchoolEnrollYear = elementarySchoolEnrollYear;
    }

    public int getSchoolType() {
        return schoolType;
    }

    public void setSchoolType(int schoolType) {
        this.schoolType = schoolType;
    }

    public String getTechSchoolName() {
        return techSchoolName;
    }

    public void setTechSchoolName(String techSchoolName) {
        this.techSchoolName = SafeFilter.getInstance().getPureContent(techSchoolName);
    }

    public int getTechSchoolCode() {
        return techSchoolCode;
    }

    public void setTechSchoolCode(int techSchoolCode) {
        this.techSchoolCode = techSchoolCode;
    }

    public int getTechSchoolEnrollYear() {
        return techSchoolEnrollYear;
    }

    public void setTechSchoolEnrollYear(int techSchoolEnrollYear) {
        this.techSchoolEnrollYear = techSchoolEnrollYear;
    }

    public String getHometownCityCode() {
        return hometownCityCode;
    }

    public void setHometownCityCode(String hometownCityCode) {
        this.hometownCityCode = SafeFilter.getInstance().getPureContent(hometownCityCode);
    }

    /**
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
                .appendSuper(super.toString())
                .append("multipartRequestHandler", this.multipartRequestHandler)
                .append("elementarySchoolEnrollYear",
                        this.elementarySchoolEnrollYear).append(
                        "highSchoolName", this.highSchoolName).append(
                        "techSchoolEnrollYear", this.techSchoolEnrollYear)
                .append("juniorSchoolEnrollYear", this.juniorSchoolEnrollYear)
                .append("techSchoolCode", this.techSchoolCode).append(
                        "juniorSchoolName", this.juniorSchoolName).append(
                        "juniorSchoolCode", this.juniorSchoolCode).append(
                        "schoolType", this.schoolType).append("servlet",
                        this.servlet).append("highSchoolCode",
                        this.highSchoolCode).append("hometownProvince",
                        this.hometownProvince).append("hometownCityCode",
                        this.hometownCityCode).append("highSchoolEnrollYear",
                        this.highSchoolEnrollYear)
                .append("gender", this.gender).append("techSchoolName",
                        this.techSchoolName).append("elementarySchoolName",
						this.elementarySchoolName).append("hometownCity",
						this.hometownCity).toString();
	}

}