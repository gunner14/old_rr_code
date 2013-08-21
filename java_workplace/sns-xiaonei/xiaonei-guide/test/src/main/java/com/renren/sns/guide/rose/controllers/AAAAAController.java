package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.IfParamExists;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.access.track.decision.Decision;
import com.xiaonei.commons.interceptors.unifiedentrance.CheckUnifiedEntrance;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.interceptors.CookieCleaner;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;
import com.xiaonei.sns.platform.core.rose.interceptors.ServiceCutter;

@Path( { "aaaaa.do" })
@LoginRequired
@Performance
@CookieCleaner
public class AAAAAController {

    @Get
    @CheckUnifiedEntrance
    public void index1() {
        System.out.println("index 1");
    }
    
    @Get("bbb.do")
    public String index3() {
        System.out.println("index 3");
        return "@aaa333";
    }

    @Get
    @ServiceCutter(timeout = 300)
    @IfParamExists(CheckUnifiedEntrance.async)
    @CheckUnifiedEntrance
    public String index(Invocation inv, Portal portal, CacheManager cm, Decision d) throws InterruptedException {
        System.out.println("index 2");
        return "@aaaaaaaaaaaaaaaaaaaaaaa";
    }

}
