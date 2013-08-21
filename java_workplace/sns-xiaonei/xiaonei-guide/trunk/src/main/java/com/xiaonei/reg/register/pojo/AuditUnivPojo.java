/**
 *
 */
package com.xiaonei.reg.register.pojo;

import java.io.Serializable;


/**
 * AuditUnivPojo.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-10 下午03:59:45
 */
public class AuditUnivPojo extends AuditBasePojo implements Serializable {
    private static final long serialVersionUID = -7412740112563388153L;

    private String department;
    private String dormitory;

    private String email;
    private String pwd;
    private String name;
    private int universityId;
    private String universityName;
    private String ip;
    private String gender;
    private String degree;
    private String appId;
    private String appToken;
    private String fromType;
    private int inviterId;
    private String groupName;

    /**
     * @param appId
     * @param appToken
     * @param cover          头像
     * @param degree
     * @param department
     * @param dormitory
     * @param email
     * @param fromType
     * @param gender
     * @param inviterId
     * @param ip
     * @param name
     * @param pwd
     * @param universityId
     * @param universityName
     */
    public AuditUnivPojo(String appId, String appToken, String degree, String department, String dormitory, String email,
                         String fromType, String gender, int inviterId, String ip, String name, String pwd, int universityId, String universityName,
                         String groupName, Upload up) {
        super(up);
        this.appId = appId;
        this.appToken = appToken;
        this.degree = degree;
        this.department = department;
        this.dormitory = dormitory;
        this.email = email;
        this.fromType = fromType;
        this.gender = gender;
        this.inviterId = inviterId;
        this.ip = ip;
        this.name = name;
        this.pwd = pwd;
        this.universityId = universityId;
        this.universityName = universityName;
        this.groupName = groupName;
    }

    public String getGroupName() {
        return this.groupName;
    }

    public String getDepartment() {
        return department;
    }

    public String getDormitory() {
        return dormitory;
    }

    public String getEmail() {
        return email;
    }

    public String getPwd() {
        return pwd;
    }

    public String getName() {
        return name;
    }

    public int getUniversityId() {
        return universityId;
    }

    public String getUniversityName() {
        return universityName;
    }

    public String getIp() {
        return ip;
    }

    public String getGender() {
        return gender;
    }

    public String getDegree() {
        return degree;
    }

    public String getAppId() {
        return appId;
    }

    public String getAppToken() {
        return appToken;
    }

    public String getFromType() {
        return fromType;
    }

    public int getInviterId() {
		return inviterId;
	}

}
