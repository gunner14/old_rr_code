package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午05:09:40
 */
@LoginRequired
@Path( { "ajaxTuiPeople", "ajaxTuiPeople.do"})
public class AjaxTuiPeopleController {

    @Get
    @Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        try{
            User user = UserProvider.getInstance().get(cm);
            GuideLogger.printLog("userId:" + user.getId());
        }
        catch(Exception e){
            e.printStackTrace();
        }        
        return "@";
    }
}
