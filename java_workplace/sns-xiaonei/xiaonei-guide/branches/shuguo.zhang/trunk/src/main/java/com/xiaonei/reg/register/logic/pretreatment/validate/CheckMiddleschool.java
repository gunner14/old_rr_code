package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.JuniorSchool;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.register.dao.CollegeDAO;
import com.xiaonei.reg.register.dao.JuniorSchoolDAO;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IMiddleschool;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import org.apache.commons.lang.StringUtils;

public class CheckMiddleschool extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IMiddleschool im;
        try {
            im = (IMiddleschool) coverInterface(regForm, IMiddleschool.class);
        } catch (Exception e) {
            e.printStackTrace();
            return OK;
        }
        IStage is;
        try {
            is = (IStage) coverInterface(regForm, IStage.class);
        } catch (Exception e) {
            return OK;
        }
        if (is.getStage() == Stage.STAGE_HEIGHSCHOOL) {
            return checkMiddleSchool(im.getSchoolType(), im.getEnrollYear(), im
                    .getHighSchoolClass(), im.getHighSchoolCode(), im
                    .getHighSchoolName(), im.getJuniorHighSchoolId(), im
                    .getJuniorHighSchoolName(), im.getTechHighSchoolId(), im
                    .getTechHighSchoolName());
        }
        return OK;
    }

    public String checkMiddleSchool(String sSchoolType, int enrollYear,
                                    String highSchoolClass, String highSchoolCode,
                                    String highSchoolName, String juniorHighSchoolId,
                                    String juniorHighSchoolName, String techHighSchoolId,
                                    String techHighSchoolName) {

        int schoolType;
        try {
            schoolType = Integer.valueOf(sSchoolType);
        } catch (NumberFormatException e) {
            e.printStackTrace();
            return ERR_NO_INPUT_HIGHSCHOOL;
        }
        if (schoolType == 0) {
            int netid;
            try {
                netid = Integer.parseInt(Escape.trim(highSchoolCode));
            } catch (NumberFormatException e) {
                return ERR_NO_INPUT_HIGHSCHOOL;
            }
            if (0 == netid || StringUtils.isEmpty(highSchoolName)) {
                return ERR_NO_INPUT_HIGHSCHOOL;
            }
            HighSchool highSchool = NetworkManager.getInstance()
                    .getHighSchoolById(netid);
            if (highSchool == null) {
                return ERR_HIGHSCHOOL_NOT_EXIST;
            }
        } else if (schoolType == 1) {
            int netid;
            try {
                netid = Integer.parseInt(Escape.trim(juniorHighSchoolId));
            } catch (NumberFormatException e) {
                return ERR_NO_INPUT_JUNIORSCHOOL;
            }
            if (0 == netid || StringUtils.isEmpty(juniorHighSchoolName)) {
                return ERR_NO_INPUT_JUNIORSCHOOL;
            }
            if (StringUtils.isEmpty(juniorHighSchoolName)) {
                return ERR_NO_INPUT_JUNIORSCHOOL;
            }
            try {
                JuniorSchool juniorSchool = JuniorSchoolDAO.getInstance()
                        .getByName(juniorHighSchoolName);
                if (juniorSchool == null) {
                    return ERR_JUNIORSCHOOL_NOT_EXIST;
                }
            } catch (Exception ex) {
                ex.printStackTrace();
                return ERR_SYSTEM;
            }
        } else if (schoolType == 2) {
            int netid;
            try {
                netid = Integer.parseInt(Escape.trim(techHighSchoolId));
            } catch (NumberFormatException e) {
                return ERR_NO_INPUT_COLLEGE;
            }
            if (0 == netid || StringUtils.isEmpty(techHighSchoolName)) {
                return ERR_NO_INPUT_COLLEGE;
            }
            if (StringUtils.isEmpty(techHighSchoolName)) {
                return ERR_NO_INPUT_COLLEGE;
            }
            try {
                College college = CollegeDAO.getInstance().getByName(techHighSchoolName);
                if (college == null) {
                    return ERR_COLLEGE_NOT_EXIST;
                }
            } catch (Exception ex) {
                ex.printStackTrace();
                return ERR_SYSTEM;
            }
        }
        return OK;
    }
}
