package com.xiaonei.reg.register.model;

import com.xiaonei.platform.core.head.HeadUrlUtil;

import java.io.Serializable;
import java.util.Date;

public class RegAuditHighSchool implements Serializable {

    private static final long serialVersionUID = 5545022239630886250L;

    public static final int AUDIT_STATUS_NEW = 0;

    public static final int AUDIT_STATUS_FAIL = 1;

    public static final int AUDIT_STATUS_SUCC = 2;

    private int id;

    private String email;

    private String password;

    private String name;

    private int highSchoolCode;

    private String highSchoolName;

    private String studentNumber;

    private int auditUser;

    private String adminName;

    private Date beginTime;

    private Date endTime;

    private int status;

    private int userId;

    private String largeurl;

    private String mainurl;

    private String headurl;

    private String tinyurl;

    private String Ip;

    private int networkId;

    private int enrollYear;

    private String highSchoolClass;

    private String gender;

    private int schoolType;

    private int inviterId;

    private String groupName;

    private int basicControl;

    public String toString() {
        return "RegAuditHighSchool:id:" + this.getId() + ",email:" + this.getEmail() + ",inviterId:" + this.inviterId + ",groupName:" + this.groupName;
    }

    public int getBasicControl() {
        return basicControl;
    }

    public void setBasicControl(int basicControl) {
        this.basicControl = basicControl;
    }

    public String getAdminName() {
        return adminName;
    }

    public int getAuditUser() {
        return auditUser;
    }

    public Date getBeginTime() {
        return beginTime;
    }

    public String getEmail() {
        return email;
    }

    public Date getEndTime() {
        return endTime;
    }

    public int getEnrollYear() {
        return enrollYear;
    }

    public String getGender() {
        return gender;
    }

    public String getHead_url() {
        return HeadUrlUtil.getHeadFullUrl(headurl);
    }

    public String getHeadurl() {
        return headurl;
    }

    public String getHighSchoolClass() {
        return highSchoolClass;
    }

    public int getHighSchoolCode() {
        return highSchoolCode;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public int getId() {
        return id;
    }

    public String getIp() {
        return Ip;
    }

    public String getLargeurl() {
        return largeurl;
    }

    public String getMainurl() {
        return mainurl;
    }

    public String getName() {
        return name;
    }

    public int getNetworkId() {
        return networkId;
    }

    public String getPassword() {
        return password;
    }

    public int getSchoolType() {
        return schoolType;
    }

    public int getStatus() {
        return status;
    }

    public String getStudentNumber() {
        return studentNumber;
    }

    public String getTinyurl() {
        return tinyurl;
    }

    public int getUserId() {
        return userId;
    }

    public void setAdminName(String adminName) {
        this.adminName = adminName;
    }

    public void setAuditUser(int auditUser) {
        this.auditUser = auditUser;
    }

    public void setBeginTime(Date beginTime) {
        this.beginTime = beginTime;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setEndTime(Date endTime) {
        this.endTime = endTime;
    }

    public void setEnrollYear(int enrollYear) {
        this.enrollYear = enrollYear;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public void setHeadurl(String headurl) {
        this.headurl = headurl;
    }

    public void setHighSchoolClass(String highSchoolClass) {
        this.highSchoolClass = highSchoolClass;
    }

    public void setHighSchoolCode(int highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setIp(String ip) {
        Ip = ip;
    }

    public void setLargeurl(String largeurl) {
        this.largeurl = largeurl;
    }

    public void setMainurl(String mainurl) {
        this.mainurl = mainurl;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setNetworkId(int networkId) {
        this.networkId = networkId;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setSchoolType(int schoolType) {
        this.schoolType = schoolType;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public void setStudentNumber(String studentNumber) {
        this.studentNumber = studentNumber;
    }

    public void setTinyurl(String tinyurl) {
        this.tinyurl = tinyurl;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

}