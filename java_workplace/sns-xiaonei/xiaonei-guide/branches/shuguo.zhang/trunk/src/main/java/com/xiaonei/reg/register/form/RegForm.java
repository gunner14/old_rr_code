package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.form.base.userganerate.IAudit;
import com.xiaonei.reg.register.form.base.userganerate.IGender;
import com.xiaonei.reg.register.form.base.userganerate.IHomecity;
import com.xiaonei.reg.register.form.base.userganerate.IInvite;
import com.xiaonei.reg.register.form.base.userganerate.IMiddleschool;
import com.xiaonei.reg.register.form.base.userganerate.IName;
import com.xiaonei.reg.register.form.base.userganerate.IPwd;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.form.base.userganerate.IUniversity;
import com.xiaonei.reg.register.form.base.userganerate.IWorkplace;

public class RegForm extends BaseRegForm implements IAccType, 
        IGender, IName, IPwd, IStage, IUniversity, IWorkplace,
        IMiddleschool, IHomecity, IInvite, IAudit {
    /**  */
    private static final long serialVersionUID = -2644522658824199202L;

    public int getAppId() {
        return appId;
    }

    public String getAppToken() {
        return appToken;
    }

    public int getBasicControl() {
        return basicControl;
    }

    public String getBirth_day() {
        return birth_day;
    }

    public String getBirth_month() {
        return birth_month;
    }

    public String getBirth_year() {
        return birth_year;
    }

    public String getGender() {
        return gender;
    }

    public String getGroupName() {
        return groupName;
    }

    public String getHighSchoolCode() {
        return highSchoolCode;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public String getHomecitycode() {
        return homecitycode;
    }

    public String getHomecityname() {
        return homecityname;
    }

    public String getHomeprovince() {
        return homeprovince;
    }

    public String getInviteeEmail() {
        return inviteeEmail;
    }

    public int getInviterId() {
        return inviterId;
    }

    public int getInviteType() {
        return inviteType;
    }

    public String getJuniorHighSchoolName() {
        return juniorHighSchoolName;
    }

    public String getName() {
        return name;
    }

    public String getPwd() {
        return pwd;
    }

    public String getRegEmail() {
        return regEmail;
    }

    public String getRepwd() {
        return repwd;
    }

    public String getSchoolType() {
        return schoolType;
    }

    public int getStage() {
        return stage;
    }

    public String getTechHighSchoolName() {
        return techHighSchoolName;
    }

    public String getTypeOfCourse() {
        return typeOfCourse;
    }

    public int getUniversityid() {
        return universityid;
    }

    public String getUniversityname() {
        return universityname;
    }

    public String getWorkplaceid() {
        return workplaceid;
    }

    public String getWorkplacename() {
        return workplacename;
    }

    public void setAppId(int appId) {
        this.appId = appId;
    }

    public void setAppToken(String appToken) {
        this.appToken = appToken;
    }

    public void setBasicControl(int basicControl) {
        this.basicControl = basicControl;
    }

    public void setBirth_day(String birth_day) {
        this.birth_day = birth_day;
    }

    public void setBirth_month(String birth_month) {
        this.birth_month = birth_month;
    }

    public void setBirth_year(String birth_year) {
        this.birth_year = birth_year;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public void setHighSchoolCode(String highSchoolCode) {
        this.highSchoolCode = highSchoolCode;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
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

    public void setInviteeEmail(String inviteeEmail) {
        this.inviteeEmail = inviteeEmail;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

    public void setInviteType(int inviteType) {
        this.inviteType = inviteType;
    }

    public void setJuniorHighSchoolName(String juniorHighSchoolName) {
        this.juniorHighSchoolName = juniorHighSchoolName;
    }

    public void setName(String name) {
        this.name = this.trimLine(name);
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }

    public void setRegEmail(String regEmail) {
        this.regEmail = regEmail.toLowerCase();
    }

    public void setRepwd(String repwd) {
        this.repwd = repwd;
    }

    public void setSchoolType(String schoolType) {
        this.schoolType = schoolType;
    }

    public void setStage(int stage) {
        if (0 == stage) {
            this.stage = 90;
        } else {
            this.stage = stage;
        }
    }

    public void setTechHighSchoolName(String techHighSchoolName) {
        this.techHighSchoolName = techHighSchoolName;
    }

    public void setTypeOfCourse(String typeOfCourse) {
        this.typeOfCourse = typeOfCourse;
    }

    public void setUniversityid(int universityid) {
        this.universityid = universityid;
    }

    public void setUniversityname(String universityname) {
        this.universityname = universityname;
    }

    public void setWorkplaceid(String workplaceid) {
        this.workplaceid = workplaceid;
    }

    public void setWorkplacename(String workplacename) {
        this.workplacename = workplacename;
    }

    public String getAudit() {
        return audit;
    }

    public void setAudit(String audit) {
        this.audit = audit;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getDormitory() {
        return dormitory;
    }

    public void setDormitory(String dormitory) {
        this.dormitory = dormitory;
    }

    public String getStudentnumber() {
        return studentnumber;
    }

    public void setStudentnumber(String studentnumber) {
        this.studentnumber = studentnumber;
    }

    public void setEnrollYear(int enrollYear) {
        this.enrollYear = enrollYear;
    }

    public int getEnrollYear() {
        return enrollYear;
    }

    public void setHighSchoolClass(String highSchoolClass) {
        this.highSchoolClass = highSchoolClass;
    }

    public String getHighSchoolClass() {
        return highSchoolClass;
    }

    public void setTechHighSchoolId(String techHighSchoolId) {
        this.techHighSchoolId = techHighSchoolId;
    }

    public String getTechHighSchoolId() {
        return techHighSchoolId;
    }

    public void setJuniorHighSchoolId(String juniorHighSchoolId) {
        this.juniorHighSchoolId = juniorHighSchoolId;
    }

    public String getJuniorHighSchoolId() {
        return juniorHighSchoolId;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
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

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    public void setPorid(String porid) {
        this.porid = porid;
    }

    public String getPorid() {
        return porid;
    }

    public String getNicknameId() {
        return nicknameId;
    }

    public void setNicknameId(String nicknameId) {
        this.nicknameId = nicknameId;
    }

	@Override
	public int getAccType() {
		return this.accType;
	}

	@Override
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
