package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IHeadupload;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import org.apache.commons.lang.StringUtils;

public class CheckHeadupload extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IHeadupload ih;
        try {
            ih = (IHeadupload) coverInterface(regForm, IHeadupload.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        if (StringUtils.isEmpty(ih.getPorid())) {
            return ERR_NO_INPUT_COVER;
        }
        return OK;
    }
}
