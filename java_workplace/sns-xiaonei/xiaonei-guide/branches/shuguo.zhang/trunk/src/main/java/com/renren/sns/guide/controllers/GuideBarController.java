package com.renren.sns.guide.controllers;

import java.io.IOException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.unifiedentrance.CheckUnifiedEntrance;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.interceptors.CookieCleaner;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;

/**
 * 
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * 普通网吧用户走的action，就是不包括叉5
 */
@Path({ "guidebar.do","GuideBar.do", "guideBar.do", "guidebar","GuideBar", "guideBar" })
@LoginRequired
@Performance
@CookieCleaner
@CheckUnifiedEntrance
public class GuideBarController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {     
        try {
            inv.getResponse().sendRedirect(OpiConstants.urlGuide+"/guide");
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        /*addWindow(portal, "window_guidefootprint", "guideFootPrint");
        addWindow(portal, "window_guideisconfirmed", "guideIsConfirmed");
        addWindow(portal, "window_guideinitusercounts", "guideInitUserCounts");                   
        addWindow(portal, "window_guidemyfriends", "guideMyFriends");
        addWindow(portal, "window_guidenewsfeed", "guideNewsFeed");
        addWindow(portal, "window_guiderightpages", "guideRightPages");
        addWindow(portal, "window_guidescore", "guideScore");
        addWindow(portal, "window_guidestatus", "guideStatus");
        addWindow(portal, "window_guideuserbasic", "guideUserBasic"); 
        addWindow(portal, "window_guidevip", "guideVip");
        addWindow(portal, "window_guidefridbirthday", "guideFridBirthday");   
        addWindow(portal, "window_guidebartreasure", "guideBarTreasure"); 
        if (OpiConstants.isKaixin()) {
            addWindow(portal, "window_recommend_app", "guideRecommendApp");
        }
        inv.addModel("userType", "barUser");
        try {  
            String forward = "bar";               
            return GuideForwards.getInstance().getForwardPath(forward);
        } catch (Exception e) {
            e.printStackTrace();
        }*/
        return "@";
    }   
   
}
