/**
 *
 */
package com.xiaonei.reg.register.form;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.*;
import org.apache.commons.lang.StringUtils;

/**
 * RegDogForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-1 - 下午11:18:41
 */
public class RegDogForm extends BaseRegForm implements IBaseInfo, IStage,
        IAccType,IPwd, IName, IGender {

    private static final long serialVersionUID = -1921303672753210138L;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public String getRegEmail() {
        return regEmail;
    }

    public void setRegEmail(String regEmail) {
        this.regEmail = StringUtils.lowerCase(regEmail);
    }

    public String getPwd() {
        return pwd;
    }

    public String getRepwd() {
        return repwd;
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
        this.repwd = pwd;
    }

    public void setRepwd(String repwd) {

    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = this.trimLine(name);
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
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
	public String getNicknameId() {
		return nicknameId;
	}

	@Override
	public void setNicknameId(String nicknameId) {
		this.nicknameId = nicknameId;
	}

}
