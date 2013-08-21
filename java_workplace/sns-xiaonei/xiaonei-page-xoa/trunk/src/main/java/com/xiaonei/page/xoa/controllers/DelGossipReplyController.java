package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

@Path("dgr")
public class DelGossipReplyController {

	@Get
	public String index(@Param("pageId") int pageId,
			@Param("gossipReplyId") int gossipReplyId,
			@Param("opUserId") int opUserId) {
		return "@";
	}
}
