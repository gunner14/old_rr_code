package com.renren.sns.explore.xoa.controllers.mobile.tag;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.HotTagService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class HotController {
	
	private static final Logger logger = Logger.getLogger(HotController.class);
	
	@Autowired
	private HotTagService hotTagService;

	@Get
	@Post
	public Object index(@Param("offset")int offset, @Param("size")int size){
		try{
			int totalSize = offset+size;
			List<String> tagList = hotTagService.getHotTagNames(totalSize);
			if(tagList!=null&&tagList.size()>offset){
				List<String> list = tagList.subList(offset, tagList.size());
				String[] tags = new String[list.size()];
				list.toArray(tags);
				return tags;
			}else{
				return null;
			}
		}catch (Exception e) {
			logger.error("get hot tags failure", e);
			return new XoaBizErrorBean("get hot tags error");
		}
	}
	
}
