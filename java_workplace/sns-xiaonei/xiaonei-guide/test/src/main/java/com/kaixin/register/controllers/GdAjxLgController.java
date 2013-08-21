/**
 * 
 */
package com.kaixin.register.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;

import com.kaixin.Globals;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;

/**
 * <ul>
 * <li> 开心农场统计业务日志
 * 
 * 不再使用的Action，但是一直有调用，不知道哪儿来的，把业务实现去掉了，只做了个Log输出
 */
@Path({"gdAjxLg.do", "guidePreAjaxLog.do"})
@LoginRequired
public class GdAjxLgController{

    final Logger log = Logger.getLogger(Globals.regLogName);
    @Get()
    @Post()
    public String index(Invocation inv) throws Exception {
        HttpServletRequest req = inv.getRequest();
        User host = BaseThreadUtil.currentHost(req);
        String referer = req.getHeader("Referer");
        log
                .debug(new StringBuilder("[GuidePreLogAction],ip=")
                    .append(BizFilterUtil.getClientIP(req))
                    .append(",host=")
                    .append(host != null ? host.getId() : 0)
                    .append(",referer=")
                    .append(referer).toString());
        return "r:" + (Globals.urlWww.toString() + "/Home.do");
    }
}
