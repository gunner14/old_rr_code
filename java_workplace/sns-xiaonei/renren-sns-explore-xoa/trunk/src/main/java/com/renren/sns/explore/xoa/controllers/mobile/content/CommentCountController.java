package com.renren.sns.explore.xoa.controllers.mobile.content;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.ContentService;
import com.renren.sns.explore.service.ExploreCommentService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;


/**
 * 获取评论总数的接口
 * @author weiwei.wang
 *
 */
public class CommentCountController {

	private static final Logger logger = Logger.getLogger(CommentCountController.class);
	
	@Autowired
	private ExploreCommentService exploreCommentService;
	
	@Get
	@Post
	public Object index(@Param("cid")String contentId){
		try {
			int count = exploreCommentService.getCommentCount(contentId);
			return new Integer(count);
		} catch (Exception e) {
			logger.error("get comment count error", e);
			return new XoaBizErrorBean("get comment count error");
		}
		
		
	}
	
	
}
