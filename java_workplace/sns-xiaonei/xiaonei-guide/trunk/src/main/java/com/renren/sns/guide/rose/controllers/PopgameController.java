package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午05:24:37
 */
@Path({ "popgame.do", "popgame"})
public class PopgameController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        //据看好像是弹Pop一个游戏页用的，暂且留着吧~ 2010-2-9 11:55:27
        //GuideLogger.printLog(" NOHERE! ",GuideLogger.ERROR);
        return "/pages/guide/others/guide_pop_games.jsp";
    }
}
