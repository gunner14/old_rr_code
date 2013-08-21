package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.reg.register.constants.IBasicControlDef;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IBasicControl;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckBasicControl extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IBasicControl ib;
        try {
            ib = (IBasicControl) coverInterface(regForm,
                    IBasicControl.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        switch (ib.getBasicControl()) {
            case IBasicControlDef.ALL_NETWORK:
            case IBasicControlDef.ALL_FRIENDS:
            case IBasicControlDef.ALL_PEOPLE:
                return OK;
        }
        return ERR_BAISCCONTROL_EMPTY;
    }

}
