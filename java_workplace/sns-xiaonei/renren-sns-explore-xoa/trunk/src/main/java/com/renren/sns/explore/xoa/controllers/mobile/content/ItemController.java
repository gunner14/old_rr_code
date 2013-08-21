package com.renren.sns.explore.xoa.controllers.mobile.content;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.explore.model.Content;
import com.renren.sns.explore.service.ContentService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class ItemController {

	private static final Logger logger = Logger.getLogger(ItemController.class);
	
	@Autowired
	private ContentService contentService;
	
	@Get
	@Post
	public Object index(@Param("cid")String contentId){
		try {
			Content content= contentService.getContentyId(contentId);
			return content;
		} catch (Exception e) {
			logger.error("get content failure", e);
			return new XoaBizErrorBean("get content error");
		}
	}
	
}
