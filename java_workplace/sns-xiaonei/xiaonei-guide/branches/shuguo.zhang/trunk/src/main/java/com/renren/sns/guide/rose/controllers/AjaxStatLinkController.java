package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午05:17:36
 */
@Path({ "ajaxStatLink.do", "ajaxStatLink"})
public class AjaxStatLinkController {

    @Get
    @Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        return "@";
    }
    
    
}
