package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午04:43:28
 */
@LoginRequired
@Path({ "ajaxGuideBuddyReplyNotify.do", "ajaxGuideBuddyReplyNotify"})
public class AjaxGuideBuddyReplyNotifyController {

    @Get
    @Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {       
    	/**
    	 * 为了避免404，这个类暂时还留着（accesslog ）里面已经没有此请求的记录了，虽然js代码里面还有
    	 * 但个人认为，没有调用到
    	 */
        return "@";
    }
}
