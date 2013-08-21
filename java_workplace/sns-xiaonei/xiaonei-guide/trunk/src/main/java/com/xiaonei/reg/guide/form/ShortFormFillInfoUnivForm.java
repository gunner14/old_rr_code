package com.xiaonei.reg.guide.form;

import java.io.Serializable;

public class ShortFormFillInfoUnivForm extends ShortFormFillInfoBaseForm
        implements Serializable {
    private static final long serialVersionUID = -2577098340397493034L;

    private String typeOfCourse;

    private int universityid;

    private String universityname;

    private String department;

    private String otherdepartment;

    private String highSchoolName;

    private int universityyear;

    private int highSchoolCode;

    private int highschoolyear;

    private String porId;

    private String tinyUrl;

    private String headUrl;

    private String mainUrl;

    private String largeUrl;

    public String getPorId() {
        return porId;
    }

    public void setPorId(String porId) {
        this.porId = porId;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        this.mainUrl = mainUrl;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public int getHighschoolyear() {
        return highschoolyear;
    }

    public void setHighschoolyear(int highschoolyear) {
        this.highschoolyear = highschoolyear;
    }

    public int getHighSchoolCode() {
        return highSchoolCode;
    }

    public void setHighSchoolCode(int highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    public int getUniversityyear() {
        return universityyear;
    }

    public void setUniversityyear(int universityyear) {
        this.universityyear = universityyear;
    }

    public String getTypeOfCourse() {
        return typeOfCourse;
    }

    public void setTypeOfCourse(String typeOfCourse) {
        this.typeOfCourse = typeOfCourse;
    }

    public int getUniversityid() {
        return universityid;
    }

    public void setUniversityid(int universityid) {
        this.universityid = universityid;
    }

    public String getUniversityname() {
        return universityname;
    }

    public void setUniversityname(String universityname) {
        this.universityname = universityname;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getOtherdepartment() {
        return otherdepartment;
    }

    public void setOtherdepartment(String otherdepartment) {
        this.otherdepartment = otherdepartment;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

}
