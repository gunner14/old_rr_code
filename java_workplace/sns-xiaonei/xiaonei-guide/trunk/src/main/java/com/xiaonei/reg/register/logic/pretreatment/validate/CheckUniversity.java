package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.form.base.userganerate.IUniversity;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import org.apache.commons.lang.StringUtils;

public class CheckUniversity extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IUniversity iu;
        try {
            iu = (IUniversity) coverInterface(regForm, IUniversity.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        IStage is;
        try {
            is = (IStage) coverInterface(regForm, IStage.class);
        } catch (Exception e) {
            e.printStackTrace();
            return checkUniversity(iu.getTypeOfCourse(), iu.getUniversityid(),
                    iu.getUniversityname());
        }
        if (Stage.STAGE_UNIV == is.getStage()) {
            return checkUniversity(iu.getTypeOfCourse(), iu.getUniversityid(),
                    iu.getUniversityname());
        }
        return OK;
    }

    public String checkUniversity(String typeOfCourse, int universityid,
                                  String universityname) {
        String degree = Escape.stringToHTMLString(typeOfCourse);
        // 注册加大学
        if (0 == universityid || StringUtils.isEmpty(universityname)) {
            return ERR_NO_INPUT_UNIV;
        }
        if (0 != universityid) {
            University university = NetworkManager.getInstance().getUnivById(
                    universityid);
            if (university == null) {
                return ERR_UNIV_NOT_EXIST;
            }
        }
        if (degree == null || degree.length() == 0) {
            return ERR_NO_SELECT_DEGREE;
        }
        return OK;
    }

}
