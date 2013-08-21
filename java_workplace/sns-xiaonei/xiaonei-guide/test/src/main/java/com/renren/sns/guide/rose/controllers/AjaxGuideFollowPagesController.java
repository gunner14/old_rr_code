package com.renren.sns.guide.rose.controllers;

import java.io.IOException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.page.PageException;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-18 下午03:09:21
 */
@LoginRequired
public class AjaxGuideFollowPagesController {

	
    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) {
        try{
            inv.getResponse().sendRedirect("http://page.renren.com/?ref=Guide_right_highschool");
        }
        catch (PageException e){
        	System.err.println("AjaxGuideFollowPagesController:" + e.toString());
        }
        catch (IOException e) {
            System.err.println("AjaxGuideFollowPagesController:" + e.toString());
        }
        catch (Exception e){
        	System.err.println("AjaxGuideFollowPagesController:" + e.toString());
        }
        return "@";
    }

}
