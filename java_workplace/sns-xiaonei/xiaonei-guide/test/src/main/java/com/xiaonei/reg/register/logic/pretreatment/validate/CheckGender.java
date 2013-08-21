package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IGender;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckGender extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        if (!asImplements(regForm, IGender.class)) {
            return OK;
        }
        IGender p = (IGender) regForm;
        return this.checkGender(p.getGender());
    }

    private String checkGender(String gender) {
        // 性别判断
        if (gender == null || !gender.equals("男生") && !gender.equals("女生")) {
            return ERR_SEX_NOT_EMPTY;
        }
        return OK;
    }
}
