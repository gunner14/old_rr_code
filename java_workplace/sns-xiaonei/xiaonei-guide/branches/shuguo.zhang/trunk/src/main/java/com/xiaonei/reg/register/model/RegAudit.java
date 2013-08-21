/**
 *
 */
package com.xiaonei.reg.register.model;

import com.xiaonei.platform.core.head.HeadUrlUtil;

import java.io.Serializable;
import java.util.Date;
import java.util.List;

/**
 * RegAudit.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-8 下午08:15:42
 */
public class RegAudit implements Serializable {
    private static final long serialVersionUID = -1141378564573728176L;

    public static final int AUDIT_STATUS_NEW = 0;

    public static final int AUDIT_STATUS_FAIL = 1;

    public static final int AUDIT_STATUS_SUCC = 2;

    private int id;

    private String email;

    private String password;

    private String name;

    private int universityId;

    private String universityName;

    private String department;

    private List<?> departmentList;

    private String dormitory;

    private List<?> dormitoryList;

    private String studentNumber;

    private int auditUser;

    private Date beginTime;

    private Date endTime;

    private int status;

    private int userId;

    private String channel;

    private String largeurl;

    private String mainurl;

    private String headurl;

    private String tinyurl;

    private String Ip;

    private int networkId;

    private String gender;

    private String degree;

    private String adminName;

    private int basicControl;

    private int inviterId;

    private String groupName;

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

    public int getBasicControl() {
        return basicControl;
    }

    public void setBasicControl(int basicControl) {
        this.basicControl = basicControl;
    }

    public String getAdminName() {
        return adminName;
    }

    public void setAdminName(String adminName) {
        this.adminName = adminName;
    }

    public String getDegree() {
        return degree;
    }

    public void setDegree(String degree) {
        this.degree = degree;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public int getNetworkId() {
        return networkId;
    }

    public void setNetworkId(int networkId) {
        this.networkId = networkId;
    }

    public String getIp() {
        return Ip;
    }

    public void setIp(String ip) {
        Ip = ip;
    }

    public String getStudentNumber() {
        return studentNumber;
    }

    public void setStudentNumber(String studentNumber) {
        this.studentNumber = studentNumber;
    }

    public String getHead_url() {
        return HeadUrlUtil.getHeadFullUrl(headurl);
    }

    public String getHeadurl() {
        return headurl;
    }

    public void setHeadurl(String headurl) {
        this.headurl = headurl;
    }

    public String getLargeurl() {
        return largeurl;
    }

    public void setLargeurl(String largeurl) {
        this.largeurl = largeurl;
    }

    public String getMainurl() {
        return mainurl;
    }

    public void setMainurl(String mainurl) {
        this.mainurl = mainurl;
    }

    public String getTinyurl() {
        return tinyurl;
    }

    public void setTinyurl(String tinyurl) {
        this.tinyurl = tinyurl;
    }

    public int getAuditUser() {
        return auditUser;
    }

    public void setAuditUser(int auditUser) {
        this.auditUser = auditUser;
    }

    public Date getBeginTime() {
        return beginTime;
    }

    public void setBeginTime(Date beginTime) {
        this.beginTime = beginTime;
    }

    public String getChannel() {
        return channel;
    }

    public void setChannel(String channel) {
        this.channel = channel;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public List<?> getDepartmentList() {
        return departmentList;
    }

    public void setDepartmentList(List<?> departmentList) {
        this.departmentList = departmentList;
    }

    public String getDormitory() {
        return dormitory;
    }

    public void setDormitory(String dormitory) {
        this.dormitory = dormitory;
    }

    public List<?> getDormitoryList() {
        return dormitoryList;
    }

    public void setDormitoryList(List<?> dormitoryList) {
        this.dormitoryList = dormitoryList;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public Date getEndTime() {
        return endTime;
    }

    public void setEndTime(Date endTime) {
        this.endTime = endTime;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public int getUniversityId() {
        return universityId;
    }

    public void setUniversityId(int universityId) {
        this.universityId = universityId;
    }

    public String getUniversityName() {
        return universityName;
    }

    public void setUniversityName(String universityName) {
        this.universityName = universityName;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }
}
