package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import org.apache.commons.lang.StringUtils;

public class CheckAccType extends BaseRegCheckLogic {

    @Override
    public String check(BaseRegForm regForm) {
        IAccType ia = null;
        boolean ab;
        try {
            ia = (IAccType) coverInterface(regForm, IAccType.class);
            if (ia.getAccType() == 0) {
                ab = false;
            } else
                ab = true;
        } catch (Exception e) {
            ab = false;
        }

        if (ab) {
            // account type存在
            if (ia.getAccType() == IAccountType.XID) {
                return CheckNicknameId.getProxy().check(regForm);
            } else if (ia.getAccType() == IAccountType.EMAIL) {
                return CheckRegEmail.getProxy().check(regForm);
            } else if (ia.getAccType() == IAccountType.MOBILE) {
                return CheckRegEmail.getProxy().check(regForm);
            } else {
                return ERR_MAIL_NOT_EMPTY;
            }
        } else {
            // account type 不存在
            if (StringUtils.isNotEmpty(ia.getRegEmail())) {
                // 只有邮件存在
                return CheckRegEmail.getProxy().check(regForm);
            } else if (StringUtils.isNotEmpty(ia.getNicknameId())) {
                // 只有xid存在
                return CheckNicknameId.getProxy().check(regForm);
            } else {
                return ERR_MAIL_NOT_EMPTY;
            }
        }

    }
}
