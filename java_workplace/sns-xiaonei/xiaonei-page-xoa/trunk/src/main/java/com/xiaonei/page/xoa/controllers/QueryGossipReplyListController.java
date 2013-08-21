package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.service.GossipService;

@Path("qgrl")
public class QueryGossipReplyListController {

	@Autowired
	private GossipService gossipService;
	
	/**
	 * 
	 * 不提供此接口
	 * 
	 * @param pageId
	 * @param gossipId
	 * @param offset
	 * @param limit
	 * @return
	 */
	@Get
	public String index(@Param("pageId") int pageId,
			@Param("gossipId") int gossipId, @Param("offset") int offset,
			@Param("limit") int limit) {
		return "@";
	}
}
