package com.renren.sns.explore.xoa.controllers.mobile.tag;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.RecommendServiceX;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class RecommoendController {

	private static final Logger logger = Logger
			.getLogger(RecommoendController.class);

	@Autowired
	private RecommendServiceX recommendServiceX;

	@Get
	@Post
	public Object index(@Param("uid") int userId, @Param("size")int size) {
		try {
			List<String> recommendTagList = recommendServiceX.getRecommendTags(
					userId, size);
			
			String[] tags = new String[recommendTagList.size()];
			recommendTagList.toArray(tags);
			return tags;
		} catch (Exception e) {
			logger.error("get recommend tags error", e);
			return new XoaBizErrorBean("get recommend tags error");
		}
	}

}
