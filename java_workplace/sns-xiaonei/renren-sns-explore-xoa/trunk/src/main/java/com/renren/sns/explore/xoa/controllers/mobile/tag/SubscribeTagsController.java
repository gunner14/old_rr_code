package com.renren.sns.explore.xoa.controllers.mobile.tag;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.ExploreUserService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class SubscribeTagsController {

	private static final Logger logger = Logger.getLogger(SubscribeTagsController.class);
	
	@Autowired
	private ExploreUserService exploreUserService;
	
	@Get
	@Post
	public Object index(@Param("uid") int userId, @Param("tag") String[] tags){
		try{
			if(userId==0||tags.length==0){
				return new  Boolean(false);
			}
			List<String> list = exploreUserService.subscribeTags(userId, tags);
			if(list.size()>0){
				return new Boolean(true);
			}else{
				return new  Boolean(false);
			}
		}catch (Exception e) {
			logger.error("subscribe tag failtue", e);
			return new XoaBizErrorBean("subscrive tag error");
		}
		
	}

}
