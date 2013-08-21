package com.xiaonei.reg.register.model;

import com.xiaonei.reg.register.form.RegForm;
import com.xiaonei.reg.register.pojo.Upload;
import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

import java.io.Serializable;

/**
 * UserAuditMajor
 * <p/>
 * table-name: user_audit_major
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-11-21 - 下午02:28:38
 */
public class UserAuditMajor implements Serializable {

    private static final long serialVersionUID = 8945628859275421069L;

    public static final int AUDIT_STATUS_NEW = 0;

    public static final int AUDIT_STATUS_FAIL = 1;

    public static final int AUDIT_STATUS_SUCC = 2;

    public static final int AUDIT_STATUS_AUTOLOGIN = 9;

    private int id;
    private int userid;
    private String name;
    private String password;
    private String email;
    private String gender;

    private int univ_id;
    private String univ_name;
    private int enrollyear;
    private String headurl;
    private String mainurl;
    private String largeurl;
    private String tinyurl;

    private String home_city_code;
    private String home_city_name;
    private String home_province;
    private String workplace_id;
    private String workplaces;
    private String ip;
    private int status;
    private int audit_user;

    private int invite_userid;
    private String invitee_email;
    private String inviter_group_name;
    private int app_id;
    private String app_token;
    private String invite_type;
    private int basic_control;

    private int stage;

    public UserAuditMajor(int id, int userid, String name, int univ_id, String univ_name, int enrollyear, String headurl, String mainurl, String largeurl,
                          String tinyurl, String workplace_id, String workplaces, String ip, int status, int audit_user, int invite_userid, String invitee_email,
                          String gender, String inviter_group_name, int app_id, String app_token, String invite_type, int basic_control, String home_city_code,
                          String home_city_name, String home_province, String password, String email, int stage) {
        super();
        this.id = id;
        this.userid = userid;
        this.name = name;
        this.univ_id = univ_id;
        this.univ_name = univ_name;
        this.enrollyear = enrollyear;
        this.headurl = headurl;
        this.mainurl = mainurl;
        this.largeurl = largeurl;
        this.tinyurl = tinyurl;
        this.workplace_id = workplace_id;
        this.workplaces = workplaces;
        this.ip = ip;
        this.status = status;
        this.audit_user = audit_user;
        this.invite_userid = invite_userid;
        this.gender = gender;
        this.inviter_group_name = inviter_group_name;
        this.app_id = app_id;
        this.app_token = app_token;
        this.invite_type = invite_type;
        this.basic_control = basic_control;
        this.home_city_code = home_city_code;
        this.home_city_name = home_city_name;
        this.home_province = home_province;
        this.password = password;
        this.email = email;
        this.invitee_email = invitee_email;
        this.stage = stage;
    }

    /**
     * 使用regForm构造
     *
     * @param regForm
     * @param ip
     */
    public UserAuditMajor(RegForm regForm, String ip) {
        super();
        this.name = regForm.getName();
        this.headurl = regForm.getHeadUrl();
        this.mainurl = regForm.getMainUrl();
        this.largeurl = regForm.getLargeUrl();
        this.tinyurl = regForm.getTinyUrl();
        this.workplace_id = regForm.getWorkplaceid();
        this.workplaces = regForm.getWorkplacename();
        this.ip = ip;
        this.invite_userid = regForm.getInviterId();
        this.gender = regForm.getGender();
        this.inviter_group_name = regForm.getGroupName();
        this.app_id = regForm.getAppId();
        this.app_token = regForm.getAppToken();
        this.invite_type = String.valueOf(regForm.getInviteType());
        this.basic_control = regForm.getBasicControl();
        this.home_city_code = regForm.getHomecitycode();
        this.home_city_name = regForm.getHomecitycode();
        this.home_province = regForm.getHomeprovince();
        this.password = regForm.getPwd();
        this.email = regForm.getRegEmail();
        this.invitee_email = regForm.getInviteeEmail();
        this.stage = Integer.valueOf(regForm.getStage());
    }

    /**
     * 白领重新审核
     *
     * @param audit
     * @param name
     * @param workpalces
     */
    public UserAuditMajor(UserAuditMajor audit, String name, String workplaces, Upload up) {
        super();
        this.id = audit.getId();
        this.userid = audit.getUserid();
        this.name = name;
        this.univ_id = audit.getUniv_id();
        this.univ_name = audit.getUniv_name();
        this.enrollyear = audit.getEnrollyear();
        this.headurl = up.getHead();
        this.mainurl = up.getMain();
        this.largeurl = up.getLarge();
        this.tinyurl = up.getTiny();
        this.workplace_id = "";
        this.workplaces = workplaces;
        this.ip = audit.getIp();
        this.status = audit.getStatus();
        this.audit_user = audit.getAudit_user();
        this.invite_userid = audit.getInvite_userid();
        this.gender = audit.getGender();
        this.inviter_group_name = audit.getInviter_group_name();
        this.app_id = audit.getApp_id();
        this.app_token = audit.getApp_token();
        this.invite_type = audit.getInvite_type();
        this.basic_control = audit.getBasic_control();
        this.home_city_code = audit.getHome_city_code();
        this.home_city_name = audit.getHome_city_name();
        this.home_province = audit.getHome_province();
        this.password = audit.getPassword();
        this.email = audit.getEmail();
        this.invitee_email = audit.getInvitee_email();
    }

    /**
     * 白领审核
     *
     * @param regForm
     * @param up
     * @param fromIp
     */
    public UserAuditMajor(RegForm regForm, Upload up, String fromIp) {
        this.name = regForm.getName();
        this.headurl = up.getHead();
        this.mainurl = up.getMain();
        this.largeurl = up.getLarge();
        this.tinyurl = up.getTiny();
        this.workplace_id = regForm.getWorkplaceid();
        this.workplaces = regForm.getWorkplacename();
        this.ip = fromIp;
        this.invite_userid = regForm.getInviterId();
        this.gender = regForm.getGender();
        this.inviter_group_name = regForm.getGroupName();
        this.app_id = regForm.getAppId();
        this.app_token = regForm.getAppToken();
        this.invite_type = String.valueOf(regForm.getInviteType());
        this.basic_control = regForm.getBasicControl();
        this.home_city_code = regForm.getHomecitycode();
        this.home_city_name = regForm.getHomecitycode();
        this.home_province = regForm.getHomeprovince();
        this.password = regForm.getPwd();
        this.email = regForm.getRegEmail();
        this.invitee_email = regForm.getInviteeEmail();
        this.stage = Integer.valueOf(regForm.getStage());
    }

    public int getId() {
        return id;
    }

    public int getUserid() {
        return userid;
    }

    public String getName() {
        return name;
    }

    public int getUniv_id() {
        return univ_id;
    }

    public String getUniv_name() {
        return univ_name;
    }

    public int getEnrollyear() {
        return enrollyear;
    }

    public String getHeadurl() {
        return headurl;
    }

    public String getMainurl() {
        return mainurl;
    }

    public String getLargeurl() {
        return largeurl;
    }

    public String getWorkplaces() {
        return workplaces;
    }

    public String getIp() {
        return ip;
    }

    public int getStatus() {
        return status;
    }

    public int getAudit_user() {
        return audit_user;
    }

    public int getInvite_userid() {
        return invite_userid;
    }

    public String getGender() {
        return gender;
    }

    public String getInviter_group_name() {
        return inviter_group_name;
    }

    public int getApp_id() {
        return app_id;
    }

    public String getApp_token() {
        return app_token;
    }

    public String getInvite_type() {
        return invite_type;
    }

    public int getBasic_control() {
        return basic_control;
    }

    public String getTinyeurl() {
        return tinyurl;
    }

    public String getHome_city_code() {
        return home_city_code;
    }

    public String getHome_city_name() {
        return home_city_name;
    }

    public String getHome_province() {
        return home_province;
    }

    public String getPassword() {
        return password;
    }

    public String getEmail() {
        return email;
    }

    public String getTinyurl() {
        return tinyurl;
    }

    public String getWorkplace_id() {
        return workplace_id;
    }

    public String getInvitee_email() {
        return invitee_email;
    }

    public int getStage() {
        return this.stage;
    }

    /**
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE).appendSuper(super.toString()).append("tinyurl", this.tinyurl).append("workplaces",
                this.workplaces).append("userid", this.userid).append("home_city_code", this.home_city_code).append("workplace_id", this.workplace_id).append(
                "stage", this.stage).append("headurl", this.headurl).append("password", this.password).append("invite_type", this.invite_type).append(
                "largeurl", this.largeurl).append("id", this.id).append("basic_control", this.basic_control).append("name", this.name).append("gender",
                this.gender).append("enrollyear", this.enrollyear).append("home_city_name", this.home_city_name).append("invitee_email", this.invitee_email)
                .append("status", this.status).append("univ_name", this.univ_name).append("mainurl", this.mainurl).append("home_province", this.home_province)
                .append("ip", this.ip).append("invite_userid", this.invite_userid).append("univ_id", this.univ_id).append("email", this.email).append(
                        "inviter_group_name", this.inviter_group_name).append("app_token", this.app_token).append("app_id", this.app_id).append("audit_user",
						this.audit_user).toString();
	}

}
