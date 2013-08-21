package com.renren.sns.guide.controllers;

import java.io.IOException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.Window;

import com.renren.sns.guide.utils.GuideForwards;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.access.track.decision.Decision;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.interceptors.CookieCleaner;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;
import com.xiaonei.sns.platform.core.rose.interceptors.ServiceCutter;

@Path( { "Guidexf.do", "guidexf.do", "Guidexf", "guidexf" })
@LoginRequired
@Performance
@CookieCleaner
public class GuidexfController {

    @Get
    @ServiceCutter(timeout = 300)
    public String index(Invocation inv, Portal portal, CacheManager cm, Decision d)
            throws InterruptedException {
        portal.setTimeout(300);//超时300ms就切除
        if (OpiConstants.isKaixin()) {
            addWindow(portal, "window_guideappinvite", "guideAppInvite");
            addWindow(portal, "window_guidefootprint", "guideFootPrint");
            addWindow(portal, "window_guidehotstar", "guideHotStar");
            addWindow(portal, "window_maybefrid", "guideMaybeFrid");
            addWindow(portal, "window_guidescore", "guideScore");            
            addWindow(portal, "window_guidemyfriends", "guideMyFriends");            
            addWindow(portal, "window_guideinitusercounts", "guideInitUserCounts");
            addWindow(portal, "window_guideisconfirmed", "guideIsConfirmed");
            addWindow(portal, "window_guidestatus", "guideStatus");
            addWindow(portal, "window_guidevip", "guideVip");
            addWindow(portal, "window_guideuserbasic", "guideUserBasic");
            addWindow(portal, "window_guidefridbirthday", "guideFridBirthday");
            addWindow(portal, "window_guideappconnect", "guideAppConnect");
            addWindow(portal, "window_guidegetmemcache", "guideGetMemCache");
            addWindow(portal, "window_guideappfootprint", "guideAppFootprint");
            addWindow(portal, "window_guidegetrequestmap", "guideGetRequestMap");
            addWindow(portal, "window_guidepositivefootprint", "guidePositiveFootprint");
            addWindow(portal, "window_getfriendlistwithintime", "getFriendListWithinTime");
            addWindow(portal, "window_guiderighthotstar", "guideRightHotStar");
            addWindow(portal, "window_guidebartreasure", "guideBarTreasure");
            addWindow(portal, "window_guideisxinsheng2010", "guideIsXinSheng2010");
            return GuideForwards.getInstance().getForwardPath("xf");
        } else {
            try {
                inv.getResponse().sendRedirect(OpiConstants.urlGuide + "/guide");
            } catch (IOException e) {
                e.printStackTrace();
            }
            return "@";
        }
    }

    private void addWindow(Portal portal, String name, String path) {
        try {
            Window window = portal.addWindow(name, "/window/guideWindows/" + path);
            window.set(Window.FUTURE_CANCEL_ENABLE_ATTR, false);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
