package com.renren.sns.guide.controllers;

import java.util.Random;

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
@Path( { "Guideka.do", "guideka.do", "Guideka", "guideka" })
@LoginRequired
@MergeWindowModel
@Performance
public class GuidekaController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        String forward = "kaixinApp";
        int max = 7000000;
        int min = 3000000;
        Random random = new Random();
        int playCount = random.nextInt(max) % (max - min + 1) + min;
        inv.addModel("playCount", playCount);
        return GuideForwards.getInstance().getForwardPath(forward);
    }
}
