/**
 *
 */
package com.xiaonei.reg.register.pojo;

import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.register.form.*;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegFormLogic;
import com.xiaonei.reg.register.model.*;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

import java.io.Serializable;

/**
 * RegisterPojo.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-25 下午05:59:27
 */
public class RegisterPojo implements Serializable {
	private static final long serialVersionUID = -2832423637518573758L;

	public RegisterPojo() {
		super();
	}

	/**
	 * 使用BaseRegForm 构造
	 * 
	 * @param regForm
	 */
	public RegisterPojo(BaseRegForm regForm, String ip) {
		constructor(regForm, ip);
	}

	/**
	 * 高中审核 构造
	 * 
	 * @param regAuditHs
	 *            RegAuditHighSchool
	 * @param RegAppAudit
	 *            RegAppAudit
	 */
	public RegisterPojo(RegAuditHighSchool regAuditHs,
			RegAppAuditHighSchool RegAppAudit) {
		this.appId = RegAppAudit.getApp_id();
		this.appToken = RegAppAudit.getApp_token();
		this.basicControl = regAuditHs.getBasicControl();
		this.gender = regAuditHs.getGender();
		this.highSchoolCode = String.valueOf(regAuditHs.getHighSchoolCode());
		this.highSchoolName = regAuditHs.getHighSchoolName();
		this.inviterId = regAuditHs.getInviterId();
		this.juniorHighSchoolName = regAuditHs.getHighSchoolName();
		this.name = regAuditHs.getName();
		this.pwd = regAuditHs.getPassword();
		this.regEmail = regAuditHs.getEmail();
		this.schoolType = String.valueOf(regAuditHs.getSchoolType());
		this.stage = String.valueOf(Stage.STAGE_HEIGHSCHOOL);
		this.techHighSchoolName = regAuditHs.getHighSchoolName();
		this.studentNumber = regAuditHs.getStudentNumber();
		this.tinyurl = regAuditHs.getTinyurl();
		this.largeurl = regAuditHs.getLargeurl();
		this.mainurl = regAuditHs.getMainurl();
		this.headurl = regAuditHs.getHeadurl();
		this.ip = regAuditHs.getIp();
		this.enrollYear = regAuditHs.getEnrollYear();
		this.highSchoolClass = regAuditHs.getHighSchoolClass();
		this.groupName = regAuditHs.getGroupName();
		this.inviteeEmail = regAuditHs.getEmail();
	}

	/**
	 * 高中mobile 注册 构造
	 * 
	 * @param rm
	 *            RegMobileWebInfoMap
	 */
	public RegisterPojo(RegMobileWebInfoMapHS rm) {
		this.appId = rm.getIv_app_id();
		this.appToken = String.valueOf(rm.getIv_app_token());
		this.basicControl = rm.getU_basic_control();
		this.gender = rm.getU_gender();
		this.highSchoolCode = String.valueOf(rm.getHs_schoolid());
		this.highSchoolName = rm.getHs_schoolname();
		this.juniorHighSchoolName = rm.getHs_schoolname();
		this.techHighSchoolName = rm.getHs_schoolname();
		this.enrollYear = rm.getHs_enrollyear();
		this.highSchoolClass = rm.getHs_high_school_class();
		this.name = rm.getU_name();
		this.pwd = rm.getU_password();
		this.regEmail = rm.getMobile();
		this.schoolType = String.valueOf(rm.getHs_school_type());
		this.stage = String.valueOf(Stage.STAGE_HEIGHSCHOOL);
		this.studentNumber = rm.getHs_student_number();
		this.tinyurl = rm.getHead_tinyurl();
		this.largeurl = rm.getHead_largeurl();
		this.mainurl = rm.getHead_mainurl();
		this.headurl = rm.getHead_headurl();
		this.ip = rm.getSys_ip();
		this.groupName = rm.getIv_inviter_group_name();
		this.inviterId = rm.getIv_inviter_id();
		this.inviteeEmail = rm.getIv_invitee_email();
	}

	/**
	 * 大学审核 构造
	 * 
	 * @param regAudit
	 * @param regAppAudit
	 */
	public RegisterPojo(RegAudit regAudit, RegAppAudit regAppAudit) {
		this.appId = regAppAudit.getApp_id();
		this.appToken = regAppAudit.getApp_token();
		this.department = regAudit.getDepartment();
		this.dormitory = regAudit.getDormitory();

		this.studentNumber = regAudit.getStudentNumber();
		this.basicControl = regAudit.getBasicControl();
		this.gender = regAudit.getGender();
		this.inviterId = regAudit.getInviterId();
		this.name = regAudit.getName();
		this.pwd = regAudit.getPassword();
		this.regEmail = regAudit.getEmail();
		this.stage = String.valueOf(Stage.STAGE_UNIV);
		this.typeOfCourse = regAudit.getDegree();
		this.universityid = regAudit.getUniversityId();
		this.universityname = regAudit.getUniversityName();
		this.tinyurl = regAudit.getTinyurl();
		this.largeurl = regAudit.getLargeurl();
		this.mainurl = regAudit.getMainurl();
		this.headurl = regAudit.getHeadurl();
		this.ip = regAudit.getIp();
		this.groupName = regAudit.getGroupName();
		this.inviteeEmail = regAudit.getEmail();
	}

	/**
	 * 白领审核注册
	 * 
	 * @param uam
	 */
	public RegisterPojo(UserAuditMajor uam) {
		this.appId = uam.getApp_id();
		this.appToken = uam.getApp_token();
		this.groupName = uam.getInviter_group_name();
		this.basicControl = uam.getBasic_control();
		this.gender = uam.getGender();
		this.homecitycode = uam.getHome_city_code();
		this.homecityname = uam.getHome_city_name();
		this.homeprovince = uam.getHome_province();
		this.inviterId = uam.getInvite_userid();
		this.name = uam.getName();
		this.pwd = uam.getPassword();
		this.regEmail = uam.getEmail();
		this.stage = String.valueOf(uam.getStage());
		this.workplaceid = uam.getWorkplace_id();
		this.workplacename = uam.getWorkplaces();
		this.ip = uam.getIp();
		this.inviteeEmail = uam.getInvitee_email();
		this.tinyurl = uam.getTinyurl();
		this.largeurl = uam.getLargeurl();
		this.mainurl = uam.getMainurl();
		this.headurl = uam.getHeadurl();
	}

	public RegisterPojo(RegProfilePageForm regForm, String hostIp) {
		constructor(regForm, hostIp);
	}

	private int appId;
	private String appToken;
	// 权限控制
	private int basicControl;
	// 出生年月日
	private String birth_day;
	private String birth_month;
	private String birth_year;

	private int enrollYear;
	// 性别
	private String gender;
	private String groupName;
	private String highSchoolClass;
	private String highSchoolCode;
	private String highSchoolName;
	private String juniorHighSchoolName;
	// 所在地
	private String homecitycode;
	private String homecityname;
	private String homeprovince;

	// 邀请人id
	private int inviterId;
	private String inviterName;
	private String inviteeEmail;
	private String ip;

	private String headurl;
	private String largeurl;
	private String mainurl;
	// 姓名
	private String name;

	// 密码
	private String pwd;

	// 邮箱
	private String regEmail;

	// 高中 技校 初中
	private String schoolType;

	// 阶段
	private String stage;

	private int status;

	private String studentNumber;

	private String techHighSchoolName;

	private String tinyurl;

	// 学历
	private String typeOfCourse;

	// 大学
	private int universityid;

	private String universityname;
	private String department;
	private String dormitory;

	// 工作地id
	private String workplaceid;

	// 工作地名称
	private String workplacename;
	private String actionId;
	private String ss;

	private int pageId;	
	
    public int getPageId() {
        return pageId;
    }
    
    public void setPageId(int pageId) {
        this.pageId = pageId;
    }

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

	public int getInviterId() {
		return inviterId;
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
		return StringUtils.lowerCase(regEmail);
	}

	public String getSchoolType() {
		return schoolType;
	}

	public String getStage() {
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

	public int getEnrollYear() {
		return enrollYear;
	}

	public String getHighSchoolClass() {
		return highSchoolClass;
	}

	public String getIp() {
		return ip;
	}

	public String getHeadurl() {
		return headurl;
	}

	public String getLargeurl() {
		return largeurl;
	}

	public String getMainurl() {
		return mainurl;
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

	public void setInviterName(String inviterName) {
		this.inviterName = inviterName;
	}

	public String getInviterName() {
		return inviterName;
	}

	public void setDepartment(String department) {
		this.department = department;
	}

	public String getDepartment() {
		return department;
	}

	public void setDormitory(String dormitory) {
		this.dormitory = dormitory;
	}

	public String getDormitory() {
		return dormitory;
	}

	public String getInviteeEmail() {
		return inviteeEmail;
	}

	private void constructor(BaseRegForm regForm, String ip) {
		IAccType iat;
		try {
			iat = (IAccType) cover(regForm, IAccType.class);
		} catch (Exception e) {
			iat = null;
		}
		IBirthday ib;
		try {
			ib = (IBirthday) cover(regForm, IBirthday.class);
		} catch (Exception e) {
			ib = null;
		}
		IGender ig;
		try {
			ig = (IGender) cover(regForm, IGender.class);
		} catch (Exception e) {
			ig = null;
		}
		IName in;
		try {
			in = (IName) cover(regForm, IName.class);
		} catch (Exception e) {
			in = null;
		}
		IPwd ipwd;
		try {
			ipwd = (IPwd) cover(regForm, IPwd.class);
		} catch (Exception e) {
			ipwd = null;
		}
		IStage is;
		try {
			is = (IStage) cover(regForm, IStage.class);
		} catch (Exception e) {
			is = null;
		}
		IUniversity iu;
		try {
			iu = (IUniversity) cover(regForm, IUniversity.class);
		} catch (Exception e) {
			iu = null;
		}
		IWorkplace iw = null;
		try {
			iw = (IWorkplace) cover(regForm, IWorkplace.class);
		} catch (Exception e) {
		}
		IMiddleschool im = null;
		try {
			im = (IMiddleschool) cover(regForm, IMiddleschool.class);
		} catch (Exception e) {
		}
		IHomecity ih = null;
		try {
			ih = (IHomecity) cover(regForm, IHomecity.class);
		} catch (Exception e) {
		}
		IInvite ii = null;
		try {
			ii = (IInvite) cover(regForm, IInvite.class);
		} catch (Exception e) {
		}
		IAudit ia = null;
		try {
			ia = (IAudit) cover(regForm, IAudit.class);
		} catch (Exception e) {
		}
		IHeadupload ihp = null;
		try {
			ihp = (IHeadupload) cover(regForm, IHeadupload.class);
		} catch (Exception e) {
		}
		this.constructor(iat, ib, ig, in, ipwd, is, iu, iw, im, ih, ii, ia,
				ihp, ip);
		this.actionId = regForm.getAction_id();
		this.ss = regForm.getSs();
		this.pageId = regForm.getPageId();
	}

	private Object cover(Object obj, Class<?> iface) throws Exception {
		BaseRegFormLogic formLogic = BaseRegFormLogic.getInstance();
		return formLogic.coverInterface(obj, IAccType.class);
	}

	/**
	 * constructor 把form转换成registerPojo
	 * 
	 * @author wangtai(tai.wang@opi-corp.com) Sep 5, 2009 - 1:11:45 PM
	 */
	private void constructor(IAccType iat, IBirthday ib, IGender ig, IName in,
			IPwd ipwd, IStage is, IUniversity iu, IWorkplace iw,
			IMiddleschool im, IHomecity ih, IInvite ii, IAudit ia,
			IHeadupload ihp, String ip) {
		if (iat != null) {
			this.regEmail = BaseRegCheckLogic.getRegAccount(iat);
		} else {
			System.err.println("email xid are null");
		}
		if (ii != null) {
			this.appId = ii.getAppId();
			this.appToken = ii.getAppToken();
			this.inviterId = ii.getInviterId();
			this.groupName = ii.getGroupName();
			this.inviteeEmail = ii.getInviteeEmail();
		}
		if (ib != null) {
			this.birth_day = ib.getBirth_day();
			this.birth_month = ib.getBirth_month();
			this.birth_year = ib.getBirth_year();
		}
		if (ig != null)
			this.gender = ig.getGender();
		if (ih != null) {
			this.homecitycode = ih.getHomecitycode();
			this.homecityname = ih.getHomecityname();
			this.homeprovince = ih.getHomeprovince();
		}

		if (in != null)
			this.name = in.getName();
		if (ipwd != null)
			this.pwd = ipwd.getPwd();
		if (im != null) {
			this.schoolType = im.getSchoolType();
			this.highSchoolName = im.getHighSchoolName();
			this.juniorHighSchoolName = im.getJuniorHighSchoolName();
			this.techHighSchoolName = im.getTechHighSchoolName();
			if (StringUtils.equals("1", schoolType))
				this.highSchoolCode = im.getJuniorHighSchoolId();
			else if (StringUtils.equals("2", schoolType))
				this.highSchoolCode = im.getTechHighSchoolId();
			else
				this.highSchoolCode = im.getHighSchoolCode();
		}
		if (is != null)
			this.stage = "" + is.getStage();
		if (iu != null) {
			this.typeOfCourse = iu.getTypeOfCourse();
			this.universityid = iu.getUniversityid();
			this.universityname = iu.getUniversityname();
		}
		if (iw != null) {
			this.workplaceid = iw.getWorkplaceid();
			this.workplacename = iw.getWorkplacename();
		}
		if (ihp != null) {
			this.tinyurl = ihp.getTinyUrl();
			this.largeurl = ihp.getLargeUrl();
			this.mainurl = ihp.getMainUrl();
			this.headurl = ihp.getHeadUrl();
		}
		this.ip = ip;
	}

	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
				.appendSuper(super.toString()).append("tinyurl", this.tinyurl)
				.append("groupName", this.groupName).append("inviterName",
						this.inviterName).append("typeOfCourse",
						this.typeOfCourse).append("appId", this.appId).append(
						"enrollYear", this.enrollYear).append("department",
						this.department).append("studentNumber",
						this.studentNumber).append("homecityname",
						this.homecityname).append("dormitory", this.dormitory)
				.append("universityid", this.universityid).append(
						"homeprovince", this.homeprovince).append("stage",
						this.stage).append("headurl", this.headurl).append(
						"largeurl", this.largeurl).append("basicControl",
						this.basicControl).append("workplaceid",
						this.workplaceid).append("name", this.name).append(
						"gender", this.gender).append("highSchoolClass",
						this.highSchoolClass).append("juniorHighSchoolName",
						this.juniorHighSchoolName).append("appToken",
						this.appToken).append("techHighSchoolName",
						this.techHighSchoolName).append("homecitycode",
						this.homecitycode).append("highSchoolName",
						this.highSchoolName).append("status", this.status)
				.append("inviterId", this.inviterId).append("mainurl",
						this.mainurl).append("workplacename",
						this.workplacename).append("schoolType",
						this.schoolType).append("universityname",
						this.universityname).append("birth_year",
						this.birth_year).append("inviteeEmail",
						this.inviteeEmail).append("ip", this.ip).append(
						"highSchoolCode", this.highSchoolCode).append("pwd",
								this.pwd).append("birth_month", this.birth_month)
				.append("regEmail", this.regEmail).append("birth_day",
						this.birth_day).toString();
	}

	public String getActionId() {
		return this.actionId;
	}

	public String getSs() {
		return this.ss;
	}
}
