package com.renren.sns.wiki.controllers.ajax;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * 前端用来判断是否登录
 * 
 * @author yi.li@renren-inc.com since 2012-5-24
 * 
 */
@Path("islogin")
public class IsLoginController extends BaseAjaxController {

    @Post("")
    @LoginRequired
    public String index() {

        return this.writeJsonData(null);
    }
}
