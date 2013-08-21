package com.xiaonei.recall_email.auto_login.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.xiaonei.recall_email.auto_login.logic.RecallKey;
import com.xiaonei.recall_email.auto_login.model.RecallEmailAutoLoginModel;

@Path({ "autoLogin", "al" })
public class AutoLoginController {

    @Get
    public String get(final Invocation inv, @Param("r") String redirect,
            @Param("k") final String key) {
        // 如果成功,登录
        final RecallEmailAutoLoginModel model = new RecallEmailAutoLoginModel();
        if (this.isValidKey(key, model)) {
            this.doLogin(model.getUserid(), inv.getRequest(), inv.getResponse());
        }
        redirect = this.getRedirect(redirect);
        return "r:" + redirect;
    }

    private void doLogin(final int userId, final HttpServletRequest requset,
            final HttpServletResponse response) {
        RecallKey.getInstance().doLogin(userId, requset, response);
    }

    private String getRedirect(final String redirect) {
        return RecallKey.getInstance().getRedirect(redirect);
    }

    private boolean isValidKey(final String key, final RecallEmailAutoLoginModel model) {
        return RecallKey.getInstance().isValidKey(key, model);
    }
}
