package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckStage extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IStage is;
        try {
            is = (IStage) coverInterface(regForm, IStage.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        switch (is.getStage()) {
            case Stage.STAGE_HEIGHSCHOOL:
            case Stage.STAGE_MAJOR:
            case Stage.STAGE_NONE:
            case Stage.STAGE_UNIV:
                return OK;
            default:
                return ERR_NONE_STAGE;
        }
    }

}
