package com.xiaonei.reg.register.controllers.init;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.reg.register.controllers.base.BaseRegInitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.RegForm;

public class Xn6401Controller extends BaseRegInitController {

    @Override
    public BaseRegForm getRegForm() {
        return new RegShortCommonForm();
    }

    @Override
    public RegForward executeRIt(RegMapping mapping, RegForm form,
            HttpServletRequest request, HttpServletResponse response) {
        return mapping.findForward("success");
    }
    
}
