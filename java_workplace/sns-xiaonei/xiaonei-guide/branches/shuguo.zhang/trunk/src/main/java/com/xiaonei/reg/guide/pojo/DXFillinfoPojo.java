package com.xiaonei.reg.guide.pojo;

import com.xiaonei.platform.core.model.User;


public class DXFillinfoPojo {

    private User host;

    private int univYear;

    private String department;

    private String otherDepartment;

    private int highSchoolId;

    private String highSchoolName;

    private int highSchoolYear;

    public int getHighSchoolYear() {
        return highSchoolYear;
    }

    public void setHighSchoolYear(int highSchoolYear) {
        this.highSchoolYear = highSchoolYear;
    }

    public User getHost() {
        return host;
    }

    public void setHost(User host) {
        this.host = host;
    }

    public int getUnivYear() {
        return univYear;
    }

    public void setUnivYear(int univYear) {
        this.univYear = univYear;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getOtherDepartment() {
        return otherDepartment;
    }

    public void setOtherDepartment(String otherDepartment) {
        this.otherDepartment = otherDepartment;
    }

    public int getHighSchoolId() {
        return highSchoolId;
    }

    public void setHighSchoolId(int highSchoolId) {
        this.highSchoolId = highSchoolId;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

}
