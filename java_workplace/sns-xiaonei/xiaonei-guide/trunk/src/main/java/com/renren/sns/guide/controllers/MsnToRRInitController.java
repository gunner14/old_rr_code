package com.renren.sns.guide.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.access.track.decision.Decision;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.interceptors.ServiceCutter;

@LoginRequired
@Path("mtrri")
public class MsnToRRInitController {

	@Autowired HostHolder hostHolder;
	
	@Get
    @ServiceCutter(timeout = 300)
    public String index(Invocation inv, Portal portal, CacheManager cm, Decision d)
            throws InterruptedException {
		User user = hostHolder.getUser();
		if(user == null){
			return "@-1";
		}
		return "msn_to_rr_init";
	}
}
