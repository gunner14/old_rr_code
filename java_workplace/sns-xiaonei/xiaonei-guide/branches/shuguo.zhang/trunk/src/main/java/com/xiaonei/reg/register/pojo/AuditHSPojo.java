/**
 *
 */
package com.xiaonei.reg.register.pojo;


/**
 * AuditHSPojo.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-10 下午06:25:21
 */
public class AuditHSPojo extends AuditBasePojo {

    private static final long serialVersionUID = -7869504683247012469L;

    //	private FormFile portait;
    private String email;
    private String pwd;
    private String name;
    private int schoolType;
    private int highSchoolCode;
    private String highSchoolName;
    private int enrollYear;
    private String highSchoolClass;
    private String ip;
    private String gender;
    private String appId;
    private String appToken;
    private String fromType;
    private String inviterId;
    private String groupName;

    /**
     * @param appId
     * @param appToken
     * @param email
     * @param enrollYear
     * @param gender
     * @param highSchoolClass
     * @param highSchoolCode
     * @param highSchoolName
     * @param fromType
     * @param inviterId
     * @param ip
     * @param name
     * @param portait
     * @param pwd
     * @param schoolType
     */
    public AuditHSPojo(String appId, String appToken, String email, int enrollYear, String gender, String highSchoolClass,
                       int highSchoolCode, String highSchoolName, String fromType, String inviterId, String ip, String name,
                       String pwd, int schoolType, String groupName, Upload up) {
        super(up);
        this.appId = appId;
        this.appToken = appToken;
        this.email = email;
        this.enrollYear = enrollYear;
        this.gender = gender;
        this.highSchoolClass = highSchoolClass;
        this.highSchoolCode = highSchoolCode;
        this.highSchoolName = highSchoolName;
        this.fromType = fromType;
        this.inviterId = inviterId;
        this.ip = ip;
        this.name = name;
//		this.portait = portait;
        this.pwd = pwd;
        this.schoolType = schoolType;
        this.groupName = groupName;
    }

    public String getGroupName() {
        return this.groupName;
    }
//	public FormFile getPortait() {
//		return portait;
//	}

    public String getEmail() {
        return email;
    }

    public String getPwd() {
        return pwd;
    }

    public String getName() {
        return name;
    }

    public int getSchoolType() {
        return schoolType;
    }

    public int getHighSchoolCode() {
        return highSchoolCode;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public int getEnrollYear() {
        return enrollYear;
    }

    public String getHighSchoolClass() {
        return highSchoolClass;
    }

    public String getIp() {
        return ip;
    }

    public String getGender() {
        return gender;
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

    public String getInviterId() {
        return inviterId;
    }

}
