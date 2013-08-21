package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.guide.logic.DigitalBeatLettersLogic;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@Path( { "aggdal" })
public class AjaxGuideGetDigitalAndLettersController {
	@Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
		String json=DigitalBeatLettersLogic.getInstance().getDigitalAndLetters();
		return "@"+json;
	}
}
