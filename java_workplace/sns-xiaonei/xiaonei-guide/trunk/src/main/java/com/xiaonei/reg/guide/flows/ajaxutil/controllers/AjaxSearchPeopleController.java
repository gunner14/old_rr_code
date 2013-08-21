package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * AjaxSearchPeopleController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明 
 */
@Path("searchpeople")
public class AjaxSearchPeopleController {
	private static final String INIT = "/views/guide/ajax/ajax_search_people.jsp";
	@Post
	@Get
	public String index(Invocation inv){
		return INIT;
	}
}
