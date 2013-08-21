package com.kaixin.register.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.annotation.AsSuperController;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;

@AsSuperController
public abstract class BaseController {
    
    public static void addMsg(HttpServletRequest request, final String msg) {
        BizFilterUtil.addMsg(request, msg);
    }
    public static void addErr(HttpServletRequest request, final String err) {
        BizFilterUtil.addErr(request, err);
    }
}
