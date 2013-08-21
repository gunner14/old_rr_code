package com.xiaonei.passport.web.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

@Path("g-signin-key")
public class GetSigninKeyController {

	@Get
	public String get(final Invocation inv, @Param("d") final String domain) {
		if ("nuomi.com".equals(domain)) {
			return "@305880804";
		} else if ("fengche.com".equals(domain)) {
			return "@QingTing";
		} else if ("56.com".equals(domain)) {
			return "@5556666";
		}
		return "@";
	}
}
