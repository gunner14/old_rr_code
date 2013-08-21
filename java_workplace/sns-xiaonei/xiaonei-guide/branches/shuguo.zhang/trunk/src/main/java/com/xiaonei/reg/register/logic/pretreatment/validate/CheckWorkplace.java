package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.form.base.userganerate.IWorkplace;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckWorkplace extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IWorkplace iw;
        try {
            iw = (IWorkplace) coverInterface(regForm, IWorkplace.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        IStage is;
        try {
            is = (IStage) coverInterface(regForm, IStage.class);
        } catch (Exception e) {
            return checkWorkplacename(iw.getWorkplacename());
        }
        if (Stage.STAGE_MAJOR == is.getStage()) {
            checkWorkplacename(iw.getWorkplacename());
        }
        return OK;
    }

    /**
     * 检测输入的公司名称
     *
     * @param workplaceName
     * @return
     */
    private String checkWorkplacename(final String workplaceName) {
        if (workplaceName != null && workplaceName.length() > 0) {
            CheckResult cr = null;
            cr = Antispam.checkAndFilterCR(workplaceName);
            if (cr.getFlag() == CheckResult.PROHIBITED) {
                return ERR_WORKPLACENAME_NOT_SAFE;
            } else if (cr.getFlag() == CheckResult.SYSTEM_ERROR) {
                System.err.println("antispam check words for pure err");
            }
        }
        return OK;
	}
}
