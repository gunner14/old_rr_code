package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午05:34:53
 */
@LoginRequired
@Path({ "GuidePre.do", "GuidePre"})
public class GuidePreController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        User host = UserProvider.getInstance().get(cm);        
        if (host != null && host.getId() % 2 != 0) {        	
            return "/pages/register/regToDog.jsp";
        } else {        	
            return "/pages/register/regToFarm.jsp";
        }
    }
}
