package com.xiaonei.reg.register.logic.additional;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.PassportManager;

public class LoginLogic {

    private static LoginLogic instance = new LoginLogic();

    private LoginLogic() {
        super();
    }

    public static LoginLogic getInstance() {
        return instance;
    }

    /**
     * @param host
     * @param request
     * @param response
     * @param type
     * @throws Exception
     */
    public void doLoginR(User host, HttpServletRequest request, HttpServletResponse response, int type, boolean isPersistent) {
        LoginLogicFactory.getPassportTicketOpr().setTicket(PassportManager.getInstance().createTicket(host.getId(), request), false, request, response);
        BaseThreadUtil.setHost(request, host);
    }

}
