/**
 *
 */
package com.xiaonei.reg.register.model;

import java.io.Serializable;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.register.constants.IRegMobileWebInfoMap;
import com.xiaonei.reg.register.pojo.RegisterPojo;

/**
 * reg_mobile_web_info_map 高中 相关数据
 * <p/>
 * RegMobileWebInfoMap_hs.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-10 下午07:15:50
 */
public class RegMobileWebInfoMapHS implements Serializable,IRegMobileWebInfoMap{

	private static final long serialVersionUID = -7586711361900443993L;

	private int id;
	private String mobile;
	private String down_code;
	private int sys_status;
	private String sys_ip;

	private String u_name;
	private int u_basic_control;
	private int u_stage;
	private int u_userid;
	private String u_gender;
	private String u_password;

	private int hs_school_type;
	private int hs_schoolid;
	private String hs_schoolname;
	private String hs_student_number;
	private int hs_enrollyear;
	private String hs_high_school_class;

	private String head_largeurl;
	private String head_mainurl;
	private String head_headurl;
	private String head_tinyurl;

	private int iv_inviter_id;
	private String iv_invitee_email;
	private int iv_app_id;
	private String iv_inviter_group_name;
	private String iv_app_token;

	/**
	 * @param down_code
	 * @param head_headurl
	 * @param head_largeurl
	 * @param head_mainurl
	 * @param head_tinyurl
	 * @param hs_enrollyear
	 * @param hs_high_school_class
	 * @param hs_school_type
	 * @param hs_schoolid
	 * @param hs_schoolname
	 * @param hs_student_number
	 * @param iv_app_id
	 * @param iv_app_token
	 * @param iv_invitee_email
	 * @param iv_inviter_group_name
	 * @param iv_inviter_id
	 * @param mobile
	 * @param sys_ip
	 * @param sys_status
	 * @param u_basic_control
	 * @param u_gender
	 * @param u_name
	 * @param u_password
	 * @param u_stage
	 * @param u_userid
	 */
	public RegMobileWebInfoMapHS(int id, String down_code, String head_headurl,
			String head_largeurl, String head_mainurl, String head_tinyurl,
			int hs_enrollyear, String hs_high_school_class, int hs_school_type,
			int hs_schoolid, String hs_schoolname, String hs_student_number,
			int iv_app_id, String iv_app_token, String iv_invitee_email,
			String iv_inviter_group_name, int iv_inviter_id, String mobile,
			String sys_ip, int sys_status, int u_basic_control,
			String u_gender, String u_name, String u_password, int u_stage,
			int u_userid) {
		this.id = id;
		this.down_code = down_code;
		this.head_headurl = head_headurl;
		this.head_largeurl = head_largeurl;
		this.head_mainurl = head_mainurl;
		this.head_tinyurl = head_tinyurl;
		this.hs_enrollyear = hs_enrollyear;
		this.hs_high_school_class = hs_high_school_class;
		this.hs_school_type = hs_school_type;
		this.hs_schoolid = hs_schoolid;
		this.hs_schoolname = hs_schoolname;
		this.hs_student_number = hs_student_number;
		this.iv_app_id = iv_app_id;
		this.iv_app_token = iv_app_token;
		this.iv_invitee_email = iv_invitee_email;
		this.iv_inviter_group_name = iv_inviter_group_name;
		this.iv_inviter_id = iv_inviter_id;
		this.mobile = mobile;
		this.sys_ip = sys_ip;
		this.sys_status = sys_status;
		this.u_basic_control = u_basic_control;
		this.u_gender = u_gender;
		this.u_name = u_name;
		this.u_password = u_password;
		this.u_stage = u_stage;
		this.u_userid = u_userid;
	}

	public RegMobileWebInfoMapHS(RegisterPojo rp, final String downCode) {
		this.down_code = downCode;
		this.head_headurl = rp.getHeadurl();
		this.head_largeurl = rp.getLargeurl();
		this.head_mainurl = rp.getMainurl();
		this.head_tinyurl = rp.getTinyurl();
		this.hs_enrollyear = rp.getEnrollYear();
		this.hs_high_school_class = rp.getHighSchoolClass();
		try {
			this.hs_school_type = Integer.valueOf(rp.getSchoolType());
		} catch (NumberFormatException e) {
			this.hs_school_type = 0;
			e.printStackTrace();
		}
		try {
			this.hs_schoolid = Integer.valueOf(rp.getHighSchoolCode());
		} catch (NumberFormatException e1) {
			this.hs_schoolid = 0;
			e1.printStackTrace();
		}
		if (0 == hs_school_type) {
			this.hs_schoolname = rp.getHighSchoolName();
		} else if (1 == hs_school_type) {
			this.hs_schoolname = rp.getJuniorHighSchoolName();
		} else if (2 == hs_school_type) {
			this.hs_schoolname = rp.getTechHighSchoolName();
		}
		this.hs_student_number = rp.getStudentNumber();
		this.iv_app_id = rp.getAppId();
		this.iv_app_token = rp.getAppToken();
		this.iv_invitee_email = rp.getInviteeEmail();
		this.iv_inviter_group_name = rp.getGroupName();
		this.iv_inviter_id = rp.getInviterId();
		this.mobile = rp.getRegEmail();
		this.sys_ip = rp.getIp();
		this.sys_status = check_new;
		this.u_basic_control = rp.getBasicControl();
		this.u_gender = rp.getGender();
		this.u_name = rp.getName();
		this.u_password = rp.getPwd();
		try {
			this.u_stage = Integer.valueOf(rp.getStage());
		} catch (Exception e) {
			this.u_stage = Stage.STAGE_HEIGHSCHOOL;
			e.printStackTrace();
		}
		this.u_userid = 0;
	}

	public String getMobile() {
		return mobile;
	}

	public String getDown_code() {
		return down_code;
	}

	public int getSys_status() {
		return sys_status;
	}

	public String getSys_ip() {
		return sys_ip;
	}

	public String getU_name() {
		return u_name;
	}

	public int getU_basic_control() {
		return u_basic_control;
	}

	public int getU_stage() {
		return u_stage;
	}

	public int getU_userid() {
		return u_userid;
	}

	public String getU_gender() {
		return u_gender;
	}

	public String getU_password() {
		return u_password;
	}

	public int getHs_school_type() {
		return hs_school_type;
	}

	public String getHs_schoolname() {
		return hs_schoolname;
	}

	public String getHs_student_number() {
		return hs_student_number;
	}

	public int getHs_enrollyear() {
		return hs_enrollyear;
	}

	public String getHs_high_school_class() {
		return hs_high_school_class;
	}

	public String getHead_largeurl() {
		return head_largeurl;
	}

	public String getHead_mainurl() {
		return head_mainurl;
	}

	public String getHead_headurl() {
		return head_headurl;
	}

	public String getHead_tinyurl() {
		return head_tinyurl;
	}

	public int getIv_inviter_id() {
		return iv_inviter_id;
	}

	public int getIv_app_id() {
		return iv_app_id;
	}

	public String getIv_inviter_group_name() {
		return iv_inviter_group_name;
	}

	public String getIv_app_token() {
		return iv_app_token;
	}

	public int getHs_schoolid() {
		return hs_schoolid;
	}

	public String getIv_invitee_email() {
		return iv_invitee_email;
	}

	public int getId() {
		return id;
	}

	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
				.appendSuper(super.toString())
				.append("u_gender", this.u_gender).append("down_code",
						this.down_code).append("hs_enrollyear",
						this.hs_enrollyear).append("head_tinyurl",
						this.head_tinyurl).append("hs_student_number",
						this.hs_student_number).append("id", this.id).append(
						"iv_inviter_group_name", this.iv_inviter_group_name)
				.append("u_userid", this.u_userid).append(
						"hs_high_school_class", this.hs_high_school_class)
				.append("u_password", this.u_password).append("u_name",
						this.u_name).append("head_headurl", this.head_headurl)
				.append("iv_app_id", this.iv_app_id).append("u_stage",
						this.u_stage).append("iv_invitee_email",
						this.iv_invitee_email).append("iv_app_token",
						this.iv_app_token).append("head_largeurl",
						this.head_largeurl).append("head_mainurl",
						this.head_mainurl).append("u_basic_control",
						this.u_basic_control).append("iv_inviter_id",
						this.iv_inviter_id).append("hs_school_type",
						this.hs_school_type).append("sys_status",
						this.sys_status)
				.append("hs_schoolid", this.hs_schoolid).append("sys_ip",
						this.sys_ip).append("mobile", this.mobile).append(
						"hs_schoolname", this.hs_schoolname).toString();
	}

}
