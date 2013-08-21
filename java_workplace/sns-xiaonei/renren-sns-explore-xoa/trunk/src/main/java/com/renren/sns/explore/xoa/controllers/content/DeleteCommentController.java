package com.renren.sns.explore.xoa.controllers.content;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.explore.service.ExploreCommentService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

/**
 *	删除探索的评论 
 * @author weiwei.wang
 */
public class DeleteCommentController {

	private static final Logger logger = Logger.getLogger(DeleteCommentController.class);
	
	@Autowired
	private ExploreCommentService exploreCommentService;
	
	@Get
	@Post
	public Object index(@Param("commentId")long commentId) {
		try {
			
			boolean result = exploreCommentService.deleteComment(commentId);
			if(result){
				return new Boolean(true);
			}else{
				return new Boolean(false);
			}
		} catch (Exception e) {
			logger.error("delete comment error", e);
			return new XoaBizErrorBean("delete comment error");
		}

	}

}
