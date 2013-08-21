package com.renren.sns.explore.xoa.controllers.mobile.content;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.explore.model.Comment;
import com.renren.sns.explore.service.ContentService;
import com.renren.sns.explore.service.ExploreCommentService;
import com.renren.xoa.commons.bean.XoaBizErrorBean;

public class CommentController {

	private static final Logger logger = Logger
			.getLogger(CommentController.class);
	
	@Autowired
	private ExploreCommentService exploreCommentService;

	@Get
	@Post
	public Object index(@Param("cid")String contentId, @Param("offset")int offset, @Param("len")int len, @Param("isDesc")boolean isDesc) {
		try {
			List<Comment> commentList = exploreCommentService.getCommentList(contentId, offset, len, isDesc);
			Comment[] comments = new Comment[commentList.size()];
			commentList.toArray(comments);
			return comments;
		} catch (Exception e) {
			logger.error("get comment error", e);
			return new XoaBizErrorBean("get comment error");
		}

	}

}
