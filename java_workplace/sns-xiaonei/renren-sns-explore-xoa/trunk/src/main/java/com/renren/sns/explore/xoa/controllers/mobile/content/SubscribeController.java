package com.renren.sns.explore.xoa.controllers.mobile.content;

import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.explore.model.Content;
import com.renren.sns.explore.service.ContentService;
import com.renren.sns.explore.service.RecommendServiceX;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class SubscribeController {

	private static final Logger logger = Logger.getLogger(SubscribeController.class);
	
	@Autowired
	private RecommendServiceX recommendServiceX;
	
	@Autowired
	private ContentService contentService;
	
	@Get
	@Post
	public Object index(@Param("uid")int userId	,@Param("offset")int offset, @Param("len")int len, @Param("from")int[] froms, @Param("type")int[] types){
	
		try {
			List<Integer> fromList = null;
			List<Integer> typeList = null;
			if(froms.length>0){
				fromList = new ArrayList<Integer>();
				for(int from:froms){
					fromList.add(from);
				}
			}
			if(types.length>0){
				typeList = new ArrayList<Integer>();
				for(int type:types){
					typeList.add(type);
				}
			}
			List<String> contentIdList = recommendServiceX.getUserSubscribledContents(userId, offset, len, typeList, fromList);
			if(contentIdList==null){
				return null;
			}
			Content[] contents = new Content[contentIdList.size()];
			for(int i=0;i<contentIdList.size();i++){
				String contentId = contentIdList.get(i);
				Content content = contentService.getContentyId(contentId);
				contents[i] = content;
			}
			return contents;
		} catch (Exception e) {
			logger.error("get subscribe contents error", e);
			return new XoaBizErrorBean("get subscribe contents error");
		}
		
	}
	
}
