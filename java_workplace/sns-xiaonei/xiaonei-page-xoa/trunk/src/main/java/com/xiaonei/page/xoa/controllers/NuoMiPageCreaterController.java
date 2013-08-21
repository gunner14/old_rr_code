package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageRegisterService;

@Path("nmpc")
public class NuoMiPageCreaterController {

	@Autowired
	private PageRegisterService pageRegisterService;
	
	private final static Logger logger=Logger.getLogger("PAGEXOA");
	
	@Get
	@Post
	public String index(@Param("ownerId")int ownerId,@Param("pageName")String pageName){
		Page page=pageRegisterService.createPage4NuoMi(ownerId, pageName);
		
		if(page!=null){
			logger.info("new page id is:"+page.getId());
			return "@"+page.getId();
		}
		
		return "@";
	}
}
