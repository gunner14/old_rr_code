package com.kaixin.register.form;

import org.apache.struts.action.ActionForm;
import org.apache.commons.lang.builder.ReflectionToStringBuilder;

public class RegForm extends ActionForm {

	private static final long serialVersionUID = -6918521371777094757L;
	//邮箱
	private String regEmail;
	//密码
	private String pwd;
	//确认密码
	private String repwd;
	//姓名
	private String name;
	//性别
	private String gender;
	//出生年月日
	private String birth_year;
	private String birth_month;
	private String birth_day;
	//所在地
	private String homecityname;
	private String homeprovince;
	private int homecitycode;
	
	//阶段
	private String stage;
	//学校id
	private String schoolid;
	//学校名称
	private String schoolname;
	//工作地id
	private String workplaceid;
	//工作地名称
	private String workplacename;
	//权限控制
	private int basicControl;
	//验证码
	private String regcode;
	//来源URL
	private String fromUrl;
	//注册表单类型
	private String regFlag;
	//邀请相关
	//邀请人id
	private int inviterId;
	private String code;
	private String inviteeEmail;
	private String from;
	private int appId;
	private int inviteType;
	private String appToken;
	
	private String groupName;
	//用于用户行为跟踪的sub source参数
	private String ss;
	//s4
	private String b;
	private String g;
	//uuid
	private String iid;	

	public String getIid() {
		return iid;
	}

	public void setIid(String iid) {
		this.iid = iid;
	}

	public String getGroupName() {
		return groupName;
	}

	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}

	public String getHomecityname() {
		return homecityname;
	}

	public void setHomecityname(String homecityname) {
		this.homecityname = homecityname;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getPwd() {
		return pwd;
	}

	public void setPwd(String pwd) {
		this.pwd = pwd;
	}

	public String getRegcode() {
		return regcode;
	}

	public void setRegcode(String regcode) {
		this.regcode = regcode;
	}

	public String getRepwd() {
		return repwd;
	}

	public void setRepwd(String repwd) {
		this.repwd = repwd;
	}

	public String getStage() {
		return stage;
	}

	public void setStage(String stage) {
		this.stage = stage;
	}

	public String getWorkplaceid() {
		return workplaceid;
	}

	public void setWorkplaceid(String workplaceid) {
		this.workplaceid = workplaceid;
	}

	public String getWorkplacename() {
		return workplacename;
	}

	public void setWorkplacename(String workplacename) {
		this.workplacename = workplacename;
	}

	public String getGender() {
		return gender;
	}

	public void setGender(String gender) {
		this.gender = gender;
	}

	public String getRegEmail() {
		return regEmail;
	}

	public void setRegEmail(String regEmail) {
		this.regEmail = regEmail.toLowerCase();
	}

	public void setBasicControl(int basicControl) {
		this.basicControl = basicControl;
	}

	public int getBasicControl() {
		return basicControl;
	}

	public String getFromUrl() {
		return fromUrl;
	}

	public void setFromUrl(String fromUrl) {
		this.fromUrl = fromUrl;
	}

	public String getRegFlag() {
		return regFlag;
	}

	public void setRegFlag(String regFlag) {
		this.regFlag = regFlag;
	}

	public String getSchoolid() {
		return schoolid;
	}

	public void setSchoolid(String schoolid) {
		this.schoolid = schoolid;
	}

	public String getSchoolname() {
		return schoolname;
	}

	public void setSchoolname(String schoolname) {
		this.schoolname = schoolname;
	}

	public int getAppId() {
		return appId;
	}

	public void setAppId(int appId) {
		this.appId = appId;
	}

	public String getAppToken() {
		return appToken;
	}

	public void setAppToken(String appToken) {
		this.appToken = appToken;
	}

	public String getCode() {
		return code;
	}

	public void setCode(String code) {
		this.code = code;
	}

	public String getFrom() {
		return from;
	}

	public void setFrom(String from) {
		this.from = from;
	}

	public String getInviteeEmail() {
		return inviteeEmail;
	}

	public void setInviteeEmail(String inviteeEmail) {
		this.inviteeEmail = inviteeEmail;
	}

	public int getInviterId() {
		return inviterId;
	}

	public void setInviterId(int inviterId) {
		this.inviterId = inviterId;
	}

	public int getInviteType() {
		return inviteType;
	}

	public void setInviteType(int inviteType) {
		this.inviteType = inviteType;
	}

	public String getBirth_day() {
		return birth_day;
	}

	public void setBirth_day(String birth_day) {
		this.birth_day = birth_day;
	}

	public String getBirth_month() {
		return birth_month;
	}

	public void setBirth_month(String birth_month) {
		this.birth_month = birth_month;
	}

	public String getBirth_year() {
		return birth_year;
	}

	public void setBirth_year(String birth_year) {
		this.birth_year = birth_year;
	}

	public int getHomecitycode() {
		return homecitycode;
	}

	public void setHomecitycode(int homecitycode) {
		this.homecitycode = homecitycode;
	}

	public String getHomeprovince() {
		return homeprovince;
	}

	public void setHomeprovince(String homeprovince) {
		this.homeprovince = homeprovince;
	}

	public String getSs() {
		return ss;
	}

	public void setSs(String ss) {
		this.ss = ss;
	}	
	
    public String getB() {
        return b;
    }
    
    public void setB(String b) {
        this.b = b;
    }
    
    public String getG() {
        return g;
    }
    
    public void setG(String g) {
        this.g = g;
    }

    public String toString(){
	    return ReflectionToStringBuilder.toString(this);
	  }
}
