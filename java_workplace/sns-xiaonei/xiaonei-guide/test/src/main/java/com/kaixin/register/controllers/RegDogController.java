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
 * 
 * @author wei.cheng@opi-corp.com
 * @date
 * 不再使用的Action，但是一直有调用，不知道哪儿来的，把业务实现去掉了，只做了个Log输出
 */
@Path({"regDog.do"})
@LoginRequired
public class RegDogController
{
    final Logger log = Logger.getLogger(Globals.regLogName);
    
    @Get()
    @Post()
	public String index(Invocation inv)
			throws Exception
	{
	    HttpServletRequest request = inv.getRequest();
        User host = BaseThreadUtil.currentHost(request);
        String referer = request.getHeader("Referer");
        log
                .debug(new StringBuilder("[RegDogAction],ip=")
                    .append(BizFilterUtil.getClientIP(request))
                    .append(",host=")
                    .append(host != null ? host.getId() : 0)
                    .append(",referer=")
                    .append(referer).toString());
        return "r:" + (Globals.urlWww.toString() + "/Home.do");
	}	

}
