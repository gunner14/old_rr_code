package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path( { "agsdbl" })
public class AjaxGuideSendDigitalBeatLettersController {

	@Get
	public String index(Invocation inv, Portal portal, CacheManager cm)
			throws InterruptedException {
		return "/views/guide/ajax/ajax_guide_send_digital_beat_letters.jsp";
	}
}
