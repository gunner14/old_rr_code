package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import org.apache.commons.lang.StringUtils;

public class RegInviteForm extends BaseRegForm implements IStage, IAccType,
        IGender, IHomecity, IMiddleschool, IName, IPwd,
        IUniversity, IInvite {

    private static final long serialVersionUID = -6396350176177443468L;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public String getRegEmail() {
        return this.regEmail;
    }

    public void setRegEmail(String regEmail) {
        this.regEmail = StringUtils.lowerCase(regEmail);
    }

    public String getPwd() {
        return this.pwd;
    }

    public String getRepwd() {
        return this.repwd;
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }

    public void setRepwd(String repwd) {
        this.repwd = repwd;
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = this.trimLine(name);
    }

    public String getGender() {
        return this.gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getHomecitycode() {
        return this.homecitycode;
    }

    public String getHomecityname() {
        return this.homecityname;
    }

    public String getHomeprovince() {
        return this.homeprovince;
    }

    public void setHomecitycode(String homecitycode) {
        this.homecitycode = homecitycode;
    }

    public void setHomecityname(String homecityname) {
        this.homecityname = homecityname;
    }

    public void setHomeprovince(String homeprovince) {
        this.homeprovince = homeprovince;
    }

    public String getTypeOfCourse() {
        return this.typeOfCourse;
    }

    public int getUniversityid() {
        return this.universityid;
    }

    public String getUniversityname() {
        return this.universityname;
    }

    public void setTypeOfCourse(String typeOfCourse) {
        this.typeOfCourse = typeOfCourse;
    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.form.base.userganerate.IUniversity#setUniversityid
      * (int)
      */
    public void setUniversityid(int universityid) {
        this.universityid = universityid;
    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.form.base.userganerate.IUniversity#setUniversityname
      * (java.lang.String)
      */
    public void setUniversityname(String universityname) {
        this.universityname = universityname;
    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.form.base.userganerate.IMiddleschool#getEnrollYear
      * ()
      */
    public int getEnrollYear() {
        return this.enrollYear;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getHighSchoolClass()
      */
    public String getHighSchoolClass() {
        return this.highSchoolClass;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getHighSchoolCode()
      */
    public String getHighSchoolCode() {
        return this.highSchoolCode;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getHighSchoolName()
      */
    public String getHighSchoolName() {
        return this.highSchoolName;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getJuniorHighSchoolId()
      */
    public String getJuniorHighSchoolId() {
        return this.juniorHighSchoolId;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getJuniorHighSchoolName()
      */
    public String getJuniorHighSchoolName() {
        return this.juniorHighSchoolName;
    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.form.base.userganerate.IMiddleschool#getSchoolType
      * ()
      */
    public String getSchoolType() {
        return this.schoolType;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getTechHighSchoolId()
      */
    public String getTechHighSchoolId() {
        return this.techHighSchoolId;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * getTechHighSchoolName()
      */
    public String getTechHighSchoolName() {
        return this.techHighSchoolName;
    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.form.base.userganerate.IMiddleschool#setEnrollYear
      * (int)
      */
    public void setEnrollYear(int enrollYear) {
        this.enrollYear = enrollYear;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * setHighSchoolClass(java.lang.String)
      */
    public void setHighSchoolClass(String highSchoolClass) {
        this.highSchoolClass = highSchoolClass;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * setHighSchoolCode(java.lang.String)
      */
    public void setHighSchoolCode(String highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * setHighSchoolName(java.lang.String)
      */
    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * setJuniorHighSchoolId(java.lang.String)
      */
    public void setJuniorHighSchoolId(String juniorHighSchoolId) {
        this.juniorHighSchoolId = juniorHighSchoolId;
    }

    /*
      * (non-Javadoc)
      *
      * @seecom.xiaonei.reg.register.form.base.userganerate.IMiddleschool#
      * setJuniorHighSchoolName(java.lang.String)
      */
    public void setJuniorHighSchoolName(String juniorHighSchoolName) {
        this.juniorHighSchoolName = juniorHighSchoolName;
    }

    public void setSchoolType(String schoolType) {
        this.schoolType = schoolType;
    }

    public void setTechHighSchoolId(String techHighSchoolId) {
        this.techHighSchoolId = techHighSchoolId;
    }

    public void setTechHighSchoolName(String techHighSchoolName) {
        this.techHighSchoolName = techHighSchoolName;
    }

    public int getAppId() {
        return appId;
    }

    public String getAppToken() {
        return appToken;
    }

    public String getGroupName() {
        return groupName;
    }

    public int getInviteType() {
        return inviteType;
    }

    public String getInviteeEmail() {
        return inviteeEmail;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setAppId(int appId) {
        this.appId = appId;
    }

    public void setAppToken(String appToken) {
        this.appToken = appToken;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public void setInviteType(int inviteType) {
        this.inviteType = inviteType;
    }

    public void setInviteeEmail(String inviteeEmail) {
        this.inviteeEmail = inviteeEmail;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

    public String getNicknameId() {
        return this.nicknameId;
    }

    public void setNicknameId(String nicknameId) {
        this.nicknameId = StringUtils.lowerCase(nicknameId);
    }

    public int getAccType() {
        return this.accType;
    }

    public void setAccType(int accType) {
        this.accType = accType;
    }
    
    @Override
    public String getAppName() {
        return this.appName;
    }

    @Override
    public void setAppName(String appName) {
        this.appName = appName;
    }
}
