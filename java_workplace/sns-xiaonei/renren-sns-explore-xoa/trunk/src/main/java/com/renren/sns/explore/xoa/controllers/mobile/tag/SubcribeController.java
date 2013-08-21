package com.renren.sns.explore.xoa.controllers.mobile.tag;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.ExploreUserService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class SubcribeController {

	private static final Logger logger = Logger
			.getLogger(SubcribeController.class);

	@Autowired
	private ExploreUserService exploreUserService;
	
	@Get
	@Post
	public Object index(@Param("uid") int userId) {

		try {
			List<String> tagValueList = exploreUserService.getUserSubscribeTagList(userId);
			String[] tags = new String[tagValueList.size()];
			tagValueList.toArray(tags);
			return tags;
		} catch (Exception e) {
			logger.error("get subscribe tags failure", e);
			return new XoaBizErrorBean("get subscribe tags error");
		}
	}

}
