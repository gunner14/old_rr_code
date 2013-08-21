package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import org.apache.commons.lang.StringUtils;

public class RegProfilePageForm extends BaseRegForm implements IAccType,
        IPwd, IName, IGender, IInvite, IBirthday {

	private static final long serialVersionUID = -4620544425735323225L;

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

    public int getAccType() {
        return this.accType;
    }

    public void setAccType(int accType) {
        this.accType = accType;
    }

	@Override
	public String getBirth_day() {
		return birth_day;
	}

	@Override
	public String getBirth_month() {
		return birth_month;
	}

	@Override
	public String getBirth_year() {
		return birth_year;
	}

	@Override
	public void setBirth_day(String birthDay) {
		this.birth_day = birthDay;
	}

	@Override
	public void setBirth_month(String birthMonth) {
		this.birth_month = birthMonth;
	}

	@Override
	public void setBirth_year(String birthYear) {
		this.birth_year = birthYear;
	}

	@Override
	public String getNicknameId() {
		return nicknameId;
	}

	@Override
	public void setNicknameId(String nicknameId) {
		this.nicknameId = nicknameId;
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
