package com.renren.sns.explore.xoa.controllers.mobile.tag;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.ExploreTagService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class SearchController {
	
	private static final Logger logger = Logger.getLogger(SearchController.class);
	
	@Autowired
	private ExploreTagService exploreTagService;
	
	@Get
	@Post
	public Object index(@Param("s")String s){
		try{
			if(s==null||s.trim().length()==0){
				return new String[0];
			}else{
				List<String> list = exploreTagService.search(s);
				String[] tags = new String[list.size()];
				list.toArray(tags);
				return tags;
			}
		}catch (Exception e) {
			logger.error("search tag error", e);
			return new XoaBizErrorBean("search tag error");
		}
	}
}
