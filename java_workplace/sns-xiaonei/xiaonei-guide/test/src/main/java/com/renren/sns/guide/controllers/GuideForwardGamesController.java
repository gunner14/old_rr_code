package com.renren.sns.guide.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.guide.utils.GuideForwards;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.interceptors.MergeWindowModel;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;

/**
 * 
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * 
 */
@Path( { "GuideForwardGames.do", "guideForwardGames.do", "GuideForwardGames", "guideForwardGames" })
@LoginRequired
@MergeWindowModel
@Performance
public class GuideForwardGamesController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        try {
            String forward = "forwardGames";
            return GuideForwards.getInstance().getForwardPath(forward);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

}
