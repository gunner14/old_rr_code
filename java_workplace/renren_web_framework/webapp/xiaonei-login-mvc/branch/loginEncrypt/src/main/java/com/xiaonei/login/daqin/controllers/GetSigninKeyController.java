package com.xiaonei.login.daqin.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.login.daqin.services.DaqinService;

@Path("g-signin-key")
public class GetSigninKeyController {

    @Autowired
    private DaqinService daqinService;

    @Get
    public String get(final Invocation inv, @Param("d") final String domain) {
        return "@" + this.daqinService.getSigninKey(domain);
    }
}
