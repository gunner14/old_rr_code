package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IHomecity;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckHomecity extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IHomecity ih;
        try {
            ih = (IHomecity) coverInterface(regForm, IHomecity.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        IStage is;
        try {
            is = (IStage) coverInterface(regForm, IStage.class);
        } catch (Exception e) {
            return checkHomecity(ih.getHomecitycode(), ih.getHomecityname(), ih
                    .getHomeprovince());
        }
        if (Stage.STAGE_MAJOR == is.getStage()
                || Stage.STAGE_NONE == is.getStage()) {
            return checkHomecity(ih.getHomecitycode(), ih.getHomecityname(), ih
                    .getHomeprovince());
        }
        return OK;
    }

    public String checkHomecity(String homecitycode, String homecityname,
                                String homeprovince) {
        String hometownprovince = Escape.stringToHTMLString(homeprovince);
        String hometownname = Escape.stringToHTMLString(homecityname);
        homecitycode = Escape.stringToHTMLString(homecitycode);
        if ((hometownprovince == null || hometownprovince.length() == 0
                || hometownname == null || hometownname.length() == 0
                || homecitycode == null || homecitycode.length() == 0)) {
            return ERR_NO_SELECT_PLACE;
        }
        return OK;
    }

}
