package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IPwd;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckPwd extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        if (!asImplements(regForm, IPwd.class)) {
            return OK;
        }
        IPwd p = (IPwd) regForm;
        return this.checkPwd(p.getPwd(), p.getRepwd());
    }

    /**
     * 用户输入的密码检测
     *
     * @param pwd
     * @param repwd
     * @return
     */
    private String checkPwd(final String pwd, final String repwd) {
        if (pwd == null || pwd.length() == 0) {
            return ERR_PWD_NOT_EMPTY;
        }
        if (pwd.length() < 6) {
            return ERR_PWD_NOT_ENOUGH;
        }
        if (pwd.length() > 20) {
            return ERR_PWD_TOO_LONG;
        }
        if (!pwd.equals(repwd) && null != repwd) {
            return ERR_PWD_NOT_EQUAL;
        }
        return OK;
	}
}
